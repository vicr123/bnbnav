const express = require('express');
const db = require('./db');
const ws = require('./ws');
let router = express.Router();

router.use(express.json({

}));

router.get("/data", async (req, res) => {
    res.send(db.data);
});

router.post("/nodes/add", async (req, res) => {
    if (req.body.x == null || req.body.y == null || req.body.z == null) {
        res.sendStatus(400);
        return;
    }

    if (Object.values(db.data.nodes).find(item => item.x == req.body.x && item.y == req.body.y && item.z == req.body.z)) {
        res.sendStatus(400);
        return;
    }
    
    let id = db.uniqueId("nodes");
    db.data.nodes[id] = {
        x: req.body.x,
        y: req.body.y,
        z: req.body.z
    };
    db.save();

    ws.broadcast({
        type: "newNode",
        id: id,
        x: req.body.x,
        y: req.body.y,
        z: req.body.z
    });

    res.send({
        id: id
    });
});
router.post("/nodes/:id", async (req, res) => {
    let id = req.params.id
    if (!db.data.nodes[id]) {
        res.sendStatus(404);
        return;
    }

    let node = JSON.parse(JSON.stringify(db.data.nodes[id]));
    if (req.body.x != null) node.x = req.body.x;
    if (req.body.y != null) node.y = req.body.y;
    if (req.body.z != null) node.z = req.body.z;

    if (Object.values(db.data.nodes).find(item => item.x == node.x && item.y == node.y && item.z == node.z)) {
        res.sendStatus(400);
        return;
    }

    db.data.nodes[id] = node;
    db.save();

    ws.broadcast({
        type: "nodeUpdated",
        id: id,
        x: node.x,
        y: node.y,
        z: node.z
    });

    res.sendStatus(200);
});
router.delete("/nodes/:id", async (req, res) => {
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

    delete db.data.nodes[id];
    db.save();

    ws.broadcast({
        type: "nodeRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/roads/add", async (req, res) => {
    if (req.body.name == null || req.body.type == null) {
        res.sendStatus(400);
        return;
    }
    
    let id = db.uniqueId("roads");
    db.data.roads[id] = {
        name: req.body.name,
        type: req.body.type
    };
    db.save();

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
router.post("/roads/:id", async (req, res) => {
    let id = req.params.id
    if (!db.data.roads[id]) {
        res.sendStatus(404);
        return;
    }

    let road = JSON.parse(JSON.stringify(db.data.roads[id]));
    if (req.body.name != null) road.name = req.body.name;
    if (req.body.type != null) road.type = req.body.type;

    db.data.roads[id] = road;
    db.save();

    ws.broadcast({
        type: "roadUpdated",
        id: id,
        name: road.name,
        type: road.type
    });

    res.sendStatus(200);
});
router.delete("/roads/:id", async (req, res) => {
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

    delete db.data.nodes[id];
    db.save();

    ws.broadcast({
        type: "roadRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/edges/add", async (req, res) => {
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
    db.save();

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
router.delete("/edges/:id", async (req, res) => {
    let id = req.params.id
    if (!db.data.edges[id]) {
        res.sendStatus(404);
        return;
    }

    delete db.data.edges[id];
    db.save();

    ws.broadcast({
        type: "edgeRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/landmarks/add", async (req, res) => {
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
    db.save();

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
router.delete("/landmarks/:id", async (req, res) => {
    let id = req.params.id
    if (!db.data.landmarks[id]) {
        res.sendStatus(404);
        return;
    }

    delete db.data.landmarks[id];
    db.save();

    ws.broadcast({
        type: "landmarkRemoved",
        id: id
    });

    res.sendStatus(200);
});
router.post("/player/:player", async (req, res) => {
    ws.broadcast({
        type: "playerMove",
        ...req.body
    });

    res.sendStatus(200);
});
router.delete("/player/:player", async (req, res) => {
    ws.broadcast({
        type: "playerGone",
        id: req.params.player
    });

    res.sendStatus(200);
});

module.exports = router;