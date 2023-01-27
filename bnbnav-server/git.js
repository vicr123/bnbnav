const Git = require("node-git-server")
const db = require("./db")
const express = require("express");

let router = express.Router();

const repos = new Git.Git(repo => db.repoPath, {

});
repos.on('push', push => {
    push.reject(403);
});

repos.on('fetch', fetch => {
    fetch.accept();
});

router.use((req, res) => {
    req.url = req.originalUrl;
    repos.handle(req, res);
});

module.exports = router;