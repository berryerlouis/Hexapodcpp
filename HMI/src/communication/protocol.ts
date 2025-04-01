import Message from "./message.ts";
import {Cluster, Command} from "./clusters/clusterType.ts";
import {getClusterByCode, getCommandByCode} from "./clusters/clusters.ts";

export enum Direction {
    RX = "Rx",
    TX = "Tx"
}

export class DecodingError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'DecodingError';
    }
}

export default class Protocol {
    constructor() {

    }

    static decode(direction: Direction, data: string): Message {
        if (data.startsWith("<") && data.endsWith(">")) {
            const raw = data;
            // remove '<' and '>'
            data = data.slice(1, -1);

            const cluster: Cluster = getClusterByCode(data.substring(0, 2));
            data = data.substring(2);

            const command = getCommandByCode(cluster, data.substring(0, 2));
            data = data.substring(2);

            const size = parseInt(data.substring(0, 2), 16);
            data = data.substring(2);

            let params: number[] =[];
            if (size > 0) {
                params = [];
                for (let index = 0; index < size; index++) {
                    params.push(parseInt(data.substring(0, 2),16));
                    data = data.substring(2);
                }

                if (data.length > 0) {
                    throw new DecodingError(`Decoding error! incorrect size: ${raw}`);
                }
            }


            return new Message(direction, cluster.name, command.name, size, params);
        } else {
            throw new DecodingError(`Should start and end with "<" and ">": ${data}`);
        }
    }


    static encode(cluster: Cluster, command: Command, size: number = 0, params: number[] = []): string {
        let messageToEncode = '<';
        messageToEncode += parseInt(cluster.code, 16).toString(16).padStart(2, '0').toUpperCase();
        messageToEncode += parseInt(command.code, 16).toString(16).padStart(2, '0').toUpperCase();
        if(size == 0) {
            messageToEncode += '00';
        }
        else
        {
            messageToEncode += size.toString(16).padStart(2, '0').toUpperCase();
            if (params && params.length > 0) {
                for ( let param of params) {
                    messageToEncode += param.toString(16).padStart(2, '0').toUpperCase();
                }
            }
        }
        messageToEncode += '>';
        return messageToEncode;
    }
}