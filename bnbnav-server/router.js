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
        type: req.body.type
    });

    res.send({
        id: id
    });
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

module.exports = router;