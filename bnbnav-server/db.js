const fs = require('fs/promises');
const simpleGit = require('simple-git');

class Database {
    data;
    git;

    constructor() {
    }

    async load() {
        try {
            let stats = await fs.stat("repo");
            if (!stats.isDirectory()) throw new Error();

            this.git = simpleGit(`${process.cwd()}/repo`);
            this.data = JSON.parse(await fs.readFile("repo/data.json", {
                encoding: "utf-8"
            }));
        } catch (err) {
            await fs.mkdir("repo");

            this.git = simpleGit(`${process.cwd()}/repo`);
            await this.git.init();
            await fs.copyFile("data.json", "repo/data.json");
            this.data = JSON.parse(await fs.readFile("repo/data.json", {
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
        await fs.writeFile("repo/data.json", contents);
        await this.git.commit(message, "data.json", {
            "--author": `"${user.name} <${user.uuid}@bnbnav.vicr123.com>"`
        });
    }
}

module.exports = new Database();