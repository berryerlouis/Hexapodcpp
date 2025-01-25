"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const net_1 = __importDefault(require("net"));
require('dotenv').config({ path: __dirname + '/.env' });
const websocket_1 = require("./websocket");
class HexapodSocket {
    constructor() {
        this.HEXAPOD_PORT = process.env.HEXAPOD_PORT || '8080';
        this.HEXAPOD_IP_ADDRESS = process.env.HEXAPOD_IP_ADDRESS || '';
        this.hexapodSocket = new net_1.default.Socket();
    }
    connect() {
        console.log('Try to connect to hexapod...');
        this.hexapodSocket.connect(Number(this.HEXAPOD_PORT), this.HEXAPOD_IP_ADDRESS, () => {
            console.log(`Connected to hexapod ${this.HEXAPOD_IP_ADDRESS}:${this.HEXAPOD_PORT}`);
        });
        this.hexapodSocket.on('data', (data) => {
            console.log(`Received message from hexapod: ${data}`);
            (0, websocket_1.getClients)().forEach((wsClient) => {
                console.log(`Send message to websocket: ${data}`);
                wsClient.send(data.toString());
            });
        });
        this.hexapodSocket.on('close', () => {
            console.log('Connection hexapod closed');
        });
        this.hexapodSocket.on('error', (err) => {
            console.error(`Error: tcp client server is not running on ${err.message}`);
        });
    }
    write(message) {
        this.hexapodSocket.write(message);
    }
}
exports.default = HexapodSocket;
