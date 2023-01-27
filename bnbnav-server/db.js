const fs = require('fs/promises');
const simpleGit = require('simple-git');

class Database {
    data;
    git;

    constructor() {
    }

    get repoPath() {
        return `${process.cwd()}/repo`;
    }

    async load() {
        try {
            let stats = await fs.stat(this.repoPath);
            if (!stats.isDirectory()) throw new Error();

            this.git = simpleGit(this.repoPath);
            this.data = JSON.parse(await fs.readFile("repo/data.json", {
                encoding: "utf-8"
            }));
        } catch (err) {
            await fs.mkdir("repo");

            this.git = simpleGit(`${process.cwd()}/repo`);
            await this.git.init();
            await fs.copyFile("data.json", `${this.repoPath}/data.json`);
            this.data = JSON.parse(await fs.readFile(`${this.repoPath}/data.json`, {
                encoding: "utf-8"
            }));
            await this.save("Initial Commit", {
                uuid: "bnbnav",
                name: "bnbnav"
            });
        }

    }
    
    uniqueId(item) {
        let id;
        do {
            id = Math.floor(Math.random() * 2000000000);
        } while (Object.keys(this.data[item]).reduce((acc, current) => acc || (current.id == id.toString()), false));

        return id.toString();
    }

    async save(message, user) {
        let contents = JSON.stringify(this.data, null, 4);
        await fs.writeFile(`${this.repoPath}/data.json`, contents);
        await this.git.add("data.json").commit(message, "data.json", {
            "--author": `"${user.name} <${user.uuid}@bnbnav.vicr123.com>"`
        });
    }
}

module.exports = new Database();