import { Clusters } from './Cluster.js'
import { Message } from './Message.js'

export class Protocol {

    static decode(direction, data) {
        if (data.substring(0, 1) === "<" && data.substring(data.length - 1) === ">") {
            const raw = data;
            // remove '<' and '>'
            data = data.substring(1);
            data = data.substring(0, data.length - 1);

            const cluster = Clusters.getClusterByCode(data.substring(0, 2));
            data = data.substring(2);

            const command = Clusters.getCommandByCode(cluster, data.substring(0, 2));
            data = data.substring(2);

            const size = parseInt(data.substring(0, 2), 16);
            data = data.substring(2);

            let params = null;
            if (size > 0) {
                params = [];
            }

            for (let index = 0; index < size; index++) {
                params.push(data.substring(0, 2));
                data = data.substring(2);
            };

            if (data.length > 0) {
                throw new Error(`Decoding error! incorect size: ${raw}`);
            }

            return new Message().build(direction, cluster.name, command.name, size, params);
        }
        else {
            throw new Error(`Should starts and ends with "<" and ">": ${data}`);
        }
    }

    static encode(cluster, command, size, params) {
        let messageToEncode = '<';
        messageToEncode += cluster.code.toString(16).padStart(2, '0');
        messageToEncode += command.code.toString(16).padStart(2, '0');
        messageToEncode += (!size ? '00' : size.toString(16).padStart(2, '0'));
        if (params) {
            for (let param of params) {
                messageToEncode += param.toString(16).padStart(2, '0');
            }
        }
        messageToEncode += '>';
        return messageToEncode;
    }

}

