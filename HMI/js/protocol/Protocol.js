import { Clusters } from './Cluster.js'
import { Message } from './Message.js'

export class Protocol {

    static decode(data) {
        if (data.substring(0, 1) === "<" && data.substring(data.length - 1) === ">") {
            const raw = data;
            let frame = new Message();
            frame.raw = data;

            data = data.substring(1);
            data = data.substring(0, data.length - 1);

            frame.cluster = Clusters.getClusterByCode(data.substring(0, 2));
            data = data.substring(2);

            frame.command = Clusters.getCommandByCode(frame.cluster, data.substring(0, 2));
            data = data.substring(2);

            frame.size = parseInt(data.substring(0, 2), 16);
            data = data.substring(2);

            if (frame.size > 0) {
                frame.params = [];
            }

            for (let index = 0; index < frame.size; index++) {
                frame.params.push(data.substring(0, 2));
                data = data.substring(2);
            };

            if (data.length > 0) {
                throw new Error(`Decoding error! incorect size: ${raw}`);
            }

            return frame;
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

