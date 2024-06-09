import { Clusters } from './Cluster.js'
import { Message } from './Message.js'

export class Protocol {

    static NO_ERROR = 'OK';
    static ERROR = 'KO';

    constructor() {
    }

    static decode(data) {
        if (data.substring(0, 1) === "<" && data.substring(data.length - 1) === ">") {
            const raw = data;
            let frame = new Message();
            frame.raw = data;

            data = data.substring(1);
            data = data.substring(0, data.length - 1);

            frame.cluster = Clusters.findClusterByCode(data.substring(0, 2));
            if (!frame.cluster) {
                throw ('Cluster not found!');
            }
            data = data.substring(2);
            frame.command = Clusters.findCommandByCode(frame.cluster, data.substring(0, 2));
            if (!frame.command) {
                throw ('Command not found!');
            }
            data = data.substring(2);
            frame.size = parseInt(data.substring(0, 2), 16);
            if (!frame.size == undefined) {
                throw ('Size not found!');
            }
            data = data.substring(2);

            if (frame.size > 0) {
                frame.params = [];
            }

            for (let index = 0; index < frame.size; index++) {
                frame.params.push(data.substring(0, 2));
                data = data.substring(2);
            };

            if (data.length > 0) {
                throw ('Decoding error! : ' + raw);
            }

            return frame;
        }
        else {
            throw ('Should starts and ends with "<" and ">"!');
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

