import { Clusters } from './Cluster.js';
import { Message } from './Message.js';
export class Protocol {
    static decode(direction, data) {
        if (data.startsWith("<") && data.endsWith(">")) {
            const raw = data;
            // remove '<' and '>'
            data = data.slice(1, -1);
            const cluster = Clusters.getClusterByCode(data.substring(0, 2));
            data = data.substring(2);
            const command = Clusters.getCommandByCode(cluster, data.substring(0, 2));
            data = data.substring(2);
            const size = parseInt(data.substring(0, 2), 16);
            data = data.substring(2);
            let params = null;
            if (size > 0) {
                params = [];
                for (let index = 0; index < size; index++) {
                    params.push(data.substring(0, 2));
                    data = data.substring(2);
                }
                if (data.length > 0) {
                    throw new Error(`Decoding error! incorrect size: ${raw}`);
                }
            }
            return new Message().build(direction, cluster.name, command.name, size, params);
        }
        else {
            throw new Error(`Should start and end with "<" and ">": ${data}`);
        }
    }
    static encode(cluster, command, size, params) {
        let messageToEncode = '<';
        messageToEncode += cluster.code.toString(16).padStart(2, '0').toUpperCase();
        messageToEncode += command.code.toString(16).padStart(2, '0').toUpperCase();
        messageToEncode += (!size ? '00' : size.toString(16).padStart(2, '0')).toUpperCase();
        if (params) {
            for (let param of params) {
                messageToEncode += Number(param).toString(16).padStart(2, '0').toUpperCase();
            }
        }
        messageToEncode += '>';
        return messageToEncode;
    }
}
