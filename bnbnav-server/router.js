const express = require('express');
const db = require('./db');
const ws = require('./ws');
const jose = require('jose');
const cors = require('cors');
const compression = require("compression");
const Mutex = require("async-mutex").Mutex;
let router = express.Router();

const SERVER_API_VERSION = 2;

const mutex = new Mutex();

function isSameWorld(world1, world2) {
    return world1 === world2;
}

router.use(express.json({

}));
router.use(compression({

}));
router.use(cors());

router.get("/data", async (req, res) => {
    res.send(db.data);
});
router.get("/data/labels", async (req, res) => {
    // Get all the labels with coordinates
    let retval = Object.keys(db.data.landmarks).map(k => [k, db.data.landmarks[k]]).filter(ld => ld[1].type.startsWith("label-")).map(tuple => {
        let [k, ld] = tuple;
        let node = db.data.nodes[ld.node];
        return {
            name: ld.name,
            type: ld.type,
            id: k,
            x: node.x,
            y: node.y,
            z: node.z,
            world: node.world
        }
    });

    res.send(retval);
});

router.use(async (req, res, next) => {
    if (req.originalUrl.startsWith("/data")) return next();

    // Ensure that the client is using a supported version of bnbnav
    res.set("X-Bnbnav-Api-Version", `${SERVER_API_VERSION}`);
    const bnbnavApiVersion = req.header("X-Bnbnav-Api-Version");
    if (!bnbnavApiVersion) {
        return res.sendStatus(400);
    }

    const bnbnavApiVersionNumber = parseInt(bnbnavApiVersion);
    if (bnbnavApiVersionNumber < SERVER_API_VERSION) {
        return res.sendStatus(400);
    }

    let auth = req.header("Authorization");
    if (!auth) return res.sendStatus(401);
    if (!auth.startsWith("Bearer ")) return res.sendStatus(401);

    let token = auth.substring(7);

    try {
        let result = await jose.jwtVerify(token, Buffer.from(process.env["BNBNAV_JWT_TOKEN"]), {
            issuer: "bnbnav",
            maxTokenAge: 7200 //7200 seconds = 2 hours
        });

        req.user = {
            uuid: result.payload.sub,
            name: result.payload.pn,
            server: result.payload.server
        };
    } catch (error) {
        if (error.code === "ERR_JWS_SIGNATURE_VERIFICATION_FAILED" || error.code === "ERR_JWT_EXPIRED" || error.code === "ERR_JWS_INVALID") {
            return res.sendStatus(401);
        } else {
            return res.sendStatus(500);
        }
    }

    next();
})

router.use(async (req, res, next) => {
    let releaseMutex = await mutex.acquire();
    res.on("finish", releaseMutex);
    await next();
})

router.post("/nodes/add", async (req, res, next) => {
    if (!req.user.server) {
        res.sendStatus(403);
        return;
    }

    if (req.body.x == null || req.body.y == null || req.body.z == null || req.body.world == null) {
        res.sendStatus(400);
        return;
    }

    if (Object.values(db.data.nodes).find(item => item.x === req.body.x && item.y === req.body.y && item.z === req.body.z && isSameWorld(item.world, req.body.world))) {
        res.sendStatus(400);
        return;
    }
    
    let id = db.uniqueId("nodes");
    db.data.nodes[id] = {
        x: req.body.x,
        y: req.body.y,
        z: req.body.z,
        world: req.body.world
    };
    await db.save(`added node ${id} at ${req.body.world}:${req.body.x},${req.body.y},${req.body.z}`, req.user);

    ws.broadcast({
        type: "newNode",
        id: id,
        x: req.body.x,
        y: req.body.y,
        z: req.body.z,
        world: req.body.world,
        player: req.body.player
    });

    res.send({
        id: id
    });
});
router.post("/nodes/:id", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.nodes[id]) {
        res.sendStatus(404);
        return;
    }

    let node = JSON.parse(JSON.stringify(db.data.nodes[id]));
    if (req.body.x != null) node.x = req.body.x;
    if (req.body.y != null) node.y = req.body.y;
    if (req.body.z != null) node.z = req.body.z;
    if (req.body.world != null) node.world = req.body.world;

    if (Object.values(db.data.nodes).find(item => item.x == node.x && item.y == node.y && item.z == node.z && isSameWorld(item.world, node.world))) {
        res.sendStatus(400);
        return;
    }

    db.data.nodes[id] = node;
    await db.save(`updated node ${id} to ${node.world}:${node.x},${node.y},${node.z}`, req.user);

    ws.broadcast({
        type: "nodeUpdated",
        id: id,
        x: node.x,
        y: node.y,
        z: node.z,
        world: node.world
    });

    res.sendStatus(200);
});
router.delete("/nodes/:id", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.nodes[id]) {
        res.sendStatus(404);
        return;
    }

    for (let edgeId of Object.keys(db.data.edges)) {
        let edge = db.data.edges[edgeId];
        if (edge.node1 == id || edge.node2 == id) {
            delete db.data.edges[edgeId];
            ws.broadcast({
                type: "edgeRemoved",
                id: edgeId
            })
        }
    }

    for (let landmarkId of Object.keys(db.data.landmarks)) {
        let landmark = db.data.landmarks[landmarkId];
        if (landmark.node == id) {
            delete db.data.landmarks[landmarkId];
            ws.broadcast({
                type: "landmarkRemoved",
                id: landmarkId
            });
        }
    }

    if (db.data.annotations[id]) {
        Object.keys(db.data.annotations[id]).forEach(annotation => ws.broadcast({
            type: "annotationRemoved",
            node: id,
            name: annotation
        }))
        delete db.data.annotations[id];
    }

    delete db.data.nodes[id];
    await db.save(`deleted node ${id}`, req.user);

    ws.broadcast({
        type: "nodeRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/roads/add", async (req, res, next) => {
    if (req.body.name == null || req.body.type == null) {
        res.sendStatus(400);
        return;
    }
    
    let id = db.uniqueId("roads");
    db.data.roads[id] = {
        name: req.body.name,
        type: req.body.type
    };
    await db.save(`added road ${id}: ${req.body.name} (${req.body.type})`, req.user);

    ws.broadcast({
        type: "newRoad",
        id: id,
        name: req.body.name,
        roadType: req.body.type
    });

    res.send({
        id: id
    });
});
router.post("/roads/:id", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.roads[id]) {
        res.sendStatus(404);
        return;
    }

    let road = JSON.parse(JSON.stringify(db.data.roads[id]));
    if (req.body.name != null) road.name = req.body.name;
    if (req.body.type != null) road.type = req.body.type;

    db.data.roads[id] = road;
    await db.save(`updated road ${id}: ${road.name} (${road.type})`, req.user);

    ws.broadcast({
        type: "roadUpdated",
        id: id,
        name: road.name,
        roadType: road.type
    });

    res.sendStatus(200);
});
router.delete("/roads/:id", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.roads[id]) {
        res.sendStatus(404);
        return;
    }

    for (let edgeId of Object.keys(db.data.edges)) {
        let edge = db.data.edges[edgeId];
        if (edge.road == id) {
            delete db.data.edges[edgeId];
            ws.broadcast({
                type: "edgeRemoved",
                id: edgeId
            })
        }
    }

    delete db.data.roads[id];
    await db.save(`deleted road ${id}`, req.user);

    ws.broadcast({
        type: "roadRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/edges/add", async (req, res, next) => {
    if (req.body.road == null || req.body.node1 == null || req.body.node2 == null) {
        res.sendStatus(400);
        return;
    }

    if (!Object.keys(db.data.roads).includes(req.body.road.toString()) || !Object.keys(db.data.nodes).includes(req.body.node1.toString()) || !Object.keys(db.data.nodes).includes(req.body.node2.toString())) {
        res.sendStatus(400);
        return;
    }
    
    let id = db.uniqueId("edges");
    db.data.edges[id] = {
        road: req.body.road.toString(),
        node1: req.body.node1.toString(),
        node2: req.body.node2.toString()
    };
    await db.save(`added edge ${id} for ${req.body.road} from ${req.body.node1} to ${req.body.node2}`, req.user);

    ws.broadcast({
        type: "newEdge",
        id: id,
        road: req.body.road.toString(),
        node1: req.body.node1.toString(),
        node2: req.body.node2.toString()
    });

    res.send({
        id: id.toString()
    });
});
router.delete("/edges/:id", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.edges[id]) {
        res.sendStatus(404);
        return;
    }

    delete db.data.edges[id];
    await db.save(`deleted edge ${id}`, req.user);

    ws.broadcast({
        type: "edgeRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/landmarks/add", async (req, res, next) => {
    if (req.body.node == null || req.body.name == null || req.body.type == null) {
        res.sendStatus(400);
        return;
    }

    if (!Object.keys(db.data.nodes).includes(req.body.node.toString())) {
        res.sendStatus(400);
        return;
    }
    
    let id = db.uniqueId("landmarks");
    db.data.landmarks[id] = {
        name: req.body.name,
        type: req.body.type,
        node: req.body.node.toString()
    };
    await db.save(`added landmark ${id}: ${req.body.name} (${req.body.type}) on ${req.body.node}`, req.user);

    ws.broadcast({
        type: "newLandmark",
        id: id,
        name: req.body.name,
        landmarkType: req.body.type,
        node: req.body.node.toString()
    });

    res.send({
        id: id
    });
});
router.delete("/landmarks/:id", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.landmarks[id]) {
        res.sendStatus(404);
        return;
    }

    delete db.data.landmarks[id];
    await db.save(`deleted landmark ${id}`, req.user);

    ws.broadcast({
        type: "landmarkRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/nodes/:id/annotations/:name", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.nodes[id]) {
        res.sendStatus(404);
        return;
    }

    let name = req.params.name;
    let annotation = req.body;

    if (!db.data.annotations[id]) db.data.annotations[id] = {};

    db.data.annotations[id][name] = annotation;
    await db.save(`added annotation ${name} on ${id}`, req.user);

    ws.broadcast({
        type: "annotationUpdated",
        node: id,
        name: name,
        annotation: annotation
    });

    res.sendStatus(200);
});
router.delete("/nodes/:id/annotations/:name", async (req, res, next) => {
    let id = req.params.id
    if (!db.data.nodes[id]) {
        res.sendStatus(404);
        return;
    }

    let name = req.params.name;

    if (!db.data.annotations[id]) db.data.annotations[id] = {};
    if (!db.data.annotations[id][name]) {
        res.sendStatus(404);
        return;
    }

    delete db.data.annotations[id][name];
    await db.save(`added deleted ${name} on ${id}`, req.user);

    ws.broadcast({
        type: "annotationRemoved",
        node: id,
        name: name
    });

    res.sendStatus(200);
});
router.post("/player/:player", async (req, res, next) => {
    ws.broadcast({
        type: "playerMove",
        ...req.body
    });

    res.sendStatus(200);
});
router.delete("/player/:player", async (req, res, next) => {
    ws.broadcast({
        type: "playerGone",
        id: req.params.player,
    });

    res.sendStatus(200);
});
router.post("/player/:player/join", async (req, res, next) => {
    ws.broadcast({
        type: "playerJoin",
        id: req.params.player,
        ...req.body
    });

    res.sendStatus(200);
});
router.post("/chat/:player", async (req, res, next) => {
    ws.broadcast({
        type: "chat",
        id: req.params.player,
        ...req.body
    });

    res.sendStatus(200);
});


module.exports = router;