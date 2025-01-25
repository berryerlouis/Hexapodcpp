import net from 'net';
require('dotenv').config({ path: __dirname+'/.env' });
import {getClients} from './websocket';

class HexapodSocket {

    HEXAPOD_PORT: string ;
    HEXAPOD_IP_ADDRESS: string;
    hexapodSocket: net.Socket;
    constructor() {
        this.HEXAPOD_PORT = process.env.HEXAPOD_PORT || '8080';
        this.HEXAPOD_IP_ADDRESS = process.env.HEXAPOD_IP_ADDRESS || '';
        this.hexapodSocket = new net.Socket();
    }

    connect() {
        console.log('Try to connect to hexapod...');
        this.hexapodSocket.connect(Number(this.HEXAPOD_PORT), this.HEXAPOD_IP_ADDRESS, () => {
            console.log(`Connected to hexapod ${this.HEXAPOD_IP_ADDRESS}:${this.HEXAPOD_PORT}`);
        });

        this.hexapodSocket.on('data', (data) => {
            console.log(`Received message from hexapod: ${data}`);
            getClients().forEach((wsClient:any) => {
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

    write(message:string) {
        this.hexapodSocket.write(message);
    }

}


export default HexapodSocket;