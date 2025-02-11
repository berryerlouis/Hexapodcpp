import { Clusters, Cluster, Command} from './Cluster.js';
import { Message } from './Message.js';

export class Protocol {
    static decode(direction: string, data: string): Message {
        if (data.startsWith("<") && data.endsWith(">")) {
            const raw = data;
            // remove '<' and '>'
            data = data.slice(1, -1);

            const cluster: Cluster = Clusters.getClusterByCode(data.substring(0, 2));
            data = data.substring(2);

            const command = Clusters.getCommandByCode(cluster, data.substring(0, 2));
            data = data.substring(2);

            const size = parseInt(data.substring(0, 2), 16);
            data = data.substring(2);

            let params: string[] | null = null;
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
        } else {
            throw new Error(`Should start and end with "<" and ">": ${data}`);
        }
    }

    static encode(cluster: Cluster, command: Command, size: number, params: string[]): string {
        let messageToEncode = '<';
        messageToEncode += parseInt(cluster.code, 16).toString(16).padStart(2, '0').toUpperCase();
        messageToEncode += parseInt(command.code, 16).toString(16).padStart(2, '0').toUpperCase();
        messageToEncode += size.toString(16).padStart(2, '0').toUpperCase();
        if (params) {
            for ( let param of params) {
                messageToEncode += parseInt(param, 16).toString(16).padStart(2, '0').toUpperCase();
            }
        }
        messageToEncode += '>';
        return messageToEncode;
    }
}