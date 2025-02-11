var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
import { Protocol } from './Protocol.js';
export class SocketInterface {
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
    isConnected() {
        return this.socket.readyState === WebSocket.OPEN;
    }
    addCallbackRead(cb) {
        this.listOfCallbackRead.push(cb);
    }
    addCallbackWrite(cb) {
        this.listOfCallbackWrite.push(cb);
    }
    notifyRead(message) {
        console.log(`Received: ${message}`);
        this.listOfCallbackRead.forEach((cb) => {
            cb(message);
        });
    }
    notifyWrite(message) {
        console.log(`Transmit: ${message}`);
        this.listOfCallbackWrite.forEach((cb) => {
            cb(message);
        });
    }
    write(message) {
        return __awaiter(this, void 0, void 0, function* () {
            return new Promise((resolve, reject) => {
                if (this.socket.readyState === WebSocket.OPEN) {
                    this.socket.send(message.raw);
                    this.notifyWrite(message);
                    resolve(true);
                }
                else {
                    reject(false);
                }
            });
        });
    }
    catchIncomingMessage(value) {
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
                }
                catch (msg) {
                    console.error(`Decoding error: "${raw}"\n"${msg}"`);
                }
                this.buffer = this.buffer.substring(endIndex);
            }
        }
    }
}
