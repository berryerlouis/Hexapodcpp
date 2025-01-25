"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.connect = connect;
exports.getClients = getClients;
const ws_1 = __importDefault(require("ws"));
require('dotenv').config({ path: __dirname + '/.env' });
let wss;
function connect(hexapodSocket) {
    const WS_PORT = process.env.WS_PORT || '8082';
    wss = new ws_1.default.Server({ port: Number(WS_PORT) });
    wss.on('connection', (ws) => {
        console.log('New Websocket Client connected');
        hexapodSocket.connect();
        ws.on('message', (message) => {
            console.log(`Received message from websocket: ${message}`);
            console.log(`Send message to hexapod: ${message}`);
            hexapodSocket.write(message);
        });
        ws.on('close', () => {
            console.log('hexapodSocket disconnected');
        });
    });
    console.log(`WebSocket server is running on ws://localhost:${WS_PORT}`);
}
function getClients() {
    return wss.clients;
}
