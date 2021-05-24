const EventEmitter = require('events');

let websockets = [];

class WebSocket extends EventEmitter {
    socket = null;
    interval = null;

    constructor(ws) {
        super();

        this.socket = ws;
        websockets.push(this);
    
        ws.on('close', () => {
            this.emit("closed");

            let idx = websockets.indexOf(this);
            websockets.splice(idx, 1);

            clearInterval(this.interval);
        });
        ws.on('message', this.message.bind(this));

        this.interval = setInterval(() => {
            this.socket.send(JSON.stringify({
                "type": "ping"
            }));
        }, 10000);

        console.log("WS connection established");
    }

    static broadcast(data) {
        for (let socket of websockets) {
            try {
                socket.socket.send(JSON.stringify(data));
            } catch {
                //meh
            }
        }
    }
}

module.exports = WebSocket;