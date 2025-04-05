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

export type Encoding = 0xFF | 0xFFFF | 0xFFFFFF | 0xFFFFFFFF;

export default class Protocol {
    constructor() {

    }

    static decode(data: string): Message {
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


            return new Message(cluster.name, command.name, size, params);
        } else {
            throw new DecodingError(`Should start and end with "<" and ">": ${data}`);
        }
    }


    static encode(cluster: Cluster, command: Command, size: number = 0, params: number[] = [], encoding?: Encoding[]): string {
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
                if(params.length == encoding?.length) {
                    for (let i = 0; i < params.length; i++) {
                        let encode: Encoding = 0xFF;
                        if(encoding) {
                            encode = encoding[i];
                        }
                        if(params[i] < 0 ) {
                            params[i] = (params[i] & encode);
                        }

                        let encodedSize = 0;
                        if(encode == 0xFF)
                            encodedSize = 2;
                        else if(encode == 0xFFFF)
                            encodedSize = 4;
                        else if(encode == 0xFFFFFF)
                            encodedSize = 6;
                        else if(encode == 0xFFFFFFFF)
                            encodedSize = 8;

                        messageToEncode += params[i].toString(16).padStart(encodedSize, '0').toUpperCase();
                    }
                } else {
                    throw new Error(`Encoding length ${encoding?.length} does not match params length ${params.length}`);
                }
            }
        }
        messageToEncode += '>';
        return messageToEncode;
    }
}