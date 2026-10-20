import Message from "./message.ts";
import { Cluster, Command } from "./clusters/clusterType.ts";
import { getClusterByCode, getCommandByCode } from "./clusters/clusters.ts";

export enum MessageDirection {
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

            let params: number[] = [];
            if (size > 0) {
                params = [];
                for (let index = 0; index < size; index++) {
                    params.push(parseInt(data.substring(0, 2), 16));
                    data = data.substring(2);
                }

                if (data.length > 0) {
                    throw new DecodingError(`Decoding error! incorrect size: ${raw}`);
                }
            }

            return new Message(cluster.name, command.name, params);
        } else {
            throw new DecodingError(`Should start and end with "<" and ">": ${data}`);
        }
    }


    static encode(cluster: Cluster, command: Command, size: number = 0, params: number[] = [], encoding?: Encoding[]): string {
        let messageToEncode = '<';
        messageToEncode += parseInt(cluster.code, 16).toString(16).padStart(2, '0').toUpperCase();
        messageToEncode += parseInt(command.code, 16).toString(16).padStart(2, '0').toUpperCase();
        if (size === 0) {
            messageToEncode += '00';
        } else {
            messageToEncode += "ZZ";
            if (params && params.length > 0) {
                if (params.length === encoding?.length) {
                    let encoded = Protocol.getSizeofParam(params, encoding);
                    messageToEncode += Protocol.toLittleEndian(params, encoded.encodedSize);
                    messageToEncode = messageToEncode.replace('ZZ', (encoded.length).toString(16).padStart(2, '0').toUpperCase());
                } else {
                    throw new Error(`Encoding length ${encoding?.length} does not match params length ${params.length}`);
                }
            }
        }
        messageToEncode += '>';
        return messageToEncode;
    }

    private static getSizeofParam(params: number[], encoding?: Encoding[]): { encodedSize: number[], length: number } {
        let encodedSize: number[] = [];
        let length: number = 0;
        for (let i: number = 0; i < params.length; i++) {
            let encode: Encoding = 0xFF;
            if (encoding) {
                encode = encoding[i];
            }
            if (params[i] < 0) {
                params[i] = (params[i] & encode);
            }
            let encoded: number = 2;
            if (encode === 0xFF)
                encoded = 2;
            else if (encode === 0xFFFF)
                encoded = 4;
            else if (encode === 0xFFFFFF)
                encoded = 6;
            else if (encode === 0xFFFFFFFF)
                encoded = 8;
            encodedSize.push(encoded);
            length += encoded / 2;
        }
        return { encodedSize, length };
    }

    private static toLittleEndian(params: number[], encodedSize: number[]): string {
        return params.map((num, index) => {
            const hex = num.toString(16).padStart(encodedSize[index], '0');
            return hex.match(/../g)!.reverse().join('');
        }).join('').toUpperCase();
    }
}