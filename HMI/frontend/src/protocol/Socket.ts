import { Message } from './Message.js';
import { Protocol } from './Protocol.js';

type Callback = (message: any) => void;

export class SocketInterface {
    private listOfCallbackRead: Callback[];
    private listOfCallbackWrite: Callback[];
    private buffer: string;
    private socket: WebSocket;

    constructor() {
        this.listOfCallbackRead = [];
        this.listOfCallbackWrite = [];
        this.buffer = '';
        this.socket = new WebSocket('ws://192.168.1.170:8080');

        this.socket.addEventListener('open', (event) => {
            console.log('Connected to the WebSocket server');
        });

        this.socket.addEventListener('message', (event) => {
            let frame = Protocol.decode("Rx", event.data);
            frame.setDate();
            this.notifyRead(frame);
        });

        this.socket.addEventListener('close', (event) => {
            console.log('Disconnected from the WebSocket server');
        });

        this.socket.addEventListener('error', (event) => {
            console.error('WebSocket error:', event);
        });
    }

    isConnected(): boolean {
        return this.socket.readyState === WebSocket.OPEN;
    }

    addCallbackRead(cb: Callback) {
        this.listOfCallbackRead.push(cb);
    }

    addCallbackWrite(cb: Callback) {
        this.listOfCallbackWrite.push(cb);
    }

    notifyRead(message: any) {
        console.log(`Received: ${message}`);
        this.listOfCallbackRead.forEach((cb) => {
            cb(message);
        });
    }

    notifyWrite(message: any) {
        console.log(`Transmit: ${message}`);
        this.listOfCallbackWrite.forEach((cb) => {
            cb(message);
        });
    }

    async write(message: Message): Promise<boolean> {
        return new Promise((resolve, reject) => {
            if(this.socket.readyState === WebSocket.OPEN) {
                this.socket.send(message.raw);
                this.notifyWrite(message);
                resolve(true);
            }
            else {
                reject(false);
            }
        });
    }

    catchIncomingMessage(value: string) {
        this.buffer += value;

        while (this.buffer.length > 0 && this.buffer.includes('>')) {
            const startIndex = this.buffer.indexOf('<');
            const endIndex = this.buffer.indexOf('>') + 1;

            if (startIndex > 0) {
                this.buffer = this.buffer.substring(startIndex);
                continue;
            }

            const raw = this.buffer.substring(startIndex, endIndex);

            if (raw.length > 0) {
                try {
                    let frame = Protocol.decode("Rx", raw);
                    this.notifyRead(frame);
                } catch (msg) {
                    console.error(`Decoding error: "${raw}"\n"${msg}"`);
                }
                this.buffer = this.buffer.substring(endIndex);
            }
        }
    }
}