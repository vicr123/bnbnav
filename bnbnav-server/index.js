const express = require('express');
const db = require('./db');
const expressws = require('express-ws');

const router = require("./router");
const WebSocket = require("./ws");

let app = express();
expressws(app);

app.use("/api", router);
app.ws("/ws", (ws, req) => {
    new WebSocket(ws);
});

app.use(express.static("../build-bnbnav-frontend-WebAssembly-Debug/"));

let port = process.env.PORT;
if (!port) port = 4000;
app.listen(port, () => {
    console.log(`Locked and loaded: port ${port}!`);
});