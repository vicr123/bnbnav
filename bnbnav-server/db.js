
const fs = require('fs');

class Database {
    data;

    constructor() {
        this.data = JSON.parse(fs.readFileSync("data.json", {
            encoding: "utf-8"
        }));
    }
    
    uniqueId(item) {
        let id;
        do {
            id = Math.floor(Math.random() * 2000000000);
        } while (Object.keys(this.data[item]).reduce((acc, current) => acc || (current.id == id.toString()), false));

        return id.toString();
    }

    save() {
        fs.writeFileSync("data.json", JSON.stringify(this.data, null, 4));
    }
}

module.exports = new Database();