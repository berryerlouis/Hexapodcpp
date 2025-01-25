import WebSocket from 'ws';
require('dotenv').config({ path: __dirname+'/.env' });
import HexapodSocket from "./tcpclient";

let wss: WebSocket.Server;

function connect(hexapodSocket:HexapodSocket):void  {
    const WS_PORT = process.env.WS_PORT || '8082';
    wss = new WebSocket.Server({ port: Number(WS_PORT) });
    wss.on('connection', (ws: WebSocket) => {
        console.log('New Websocket Client connected');
        hexapodSocket.connect();
        ws.on('message', (message: string) => {
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

function getClients () :  Set<WebSocket> {
    return wss.clients;
}

export  {connect, getClients};