import {Clusters} from './Cluster.js'
import {Protocol} from './Protocol.js'
import {MessageSizeError} from './MessageError.js';

export class Message {
    date;
    direction;
    params;
    cluster;
    command;
    size;
    raw;
    index;
    timeout;

    build(direction, clusterName, commandName, size = 0, params = null) {
        this.direction = direction;
        this.cluster = Clusters.getClusterByName(clusterName);
        this.command = Clusters.getCommandByName(this.cluster, commandName);
        this.timeout = 0;

        if ((params != null && params.length !== size) || (params == null && size !== 0)) {
            throw new MessageSizeError(`Message with size ${size} doesn't expect the lenght of params ${params?.length}!`);
        } else {

            this.size = size;
            this.params = params;
            this.index = 0;
            this.raw = Protocol.encode(this.cluster, this.command, this.size, this.params);
        }
        return this;
    }

    setDate() {
        let date = new Date();
        this.date = new Date(date.getTime() - date.getTimezoneOffset() * 60000).toISOString().replace('T', ' ').replace('Z', ' ');
    }

    toString() {
        let ret = this.date;
        ret += '\t' + this.direction;
        ret += '\t' + this.raw;
        ret += '\t' + this.cluster.name;
        ret += '\t' + this.command.name;
        return ret;
    }

    fetchInt8U() {
        let num = Infinity
        if (this.index + 1 <= this.size) {
            num = parseInt(this.params[this.index], 16);
            this.index++;
        } else {
            throw new MessageSizeError(`Already expect the lenght of params ${this.size}!`);
        }
        return num;
    }

    fetchInt8S() {
        let num = this.fetchInt8U();
        if (num & 0x80) {
            num = -(0x100 - num);
        }
        return num;
    }

    fetchInt16U() {
        let num = Infinity
        if (this.index + 2 <= this.size) {
            num = parseInt(
                this.params[this.index + 1] +
                this.params[this.index], 16);
            this.index += 2;
        } else {
            throw new MessageSizeError(`Already expect the lenght of params ${this.size}!`);
        }
        return num;
    }

    fetchInt16S() {
        let num = this.fetchInt16U();
        if (num & 0x8000) {
            num = -(0x10000 - num);
        }
        return num;
    }

    fetchInt24U() {
        let num = Infinity
        if (this.index + 3 <= this.size) {
            num = parseInt(
                this.params[this.index + 2] +
                this.params[this.index + 1] +
                this.params[this.index], 16);
            this.index += 3;
        } else {
            throw new MessageSizeError(`Already expect the lenght of params ${this.size}!`);
        }
        return num;
    }

    fetchInt32U() {
        let num = Infinity
        if (this.index + 4 <= this.size) {
            num = parseInt(
                this.params[this.index + 3] +
                this.params[this.index + 2] +
                this.params[this.index + 1] +
                this.params[this.index], 16);
            this.index += 4;
        } else {
            throw new MessageSizeError(`Already expect the lenght of params ${this.size}!`);
        }
        return num;
    }

    fetchInt32S() {
        let num = this.fetchInt32U();
        const buf = Buffer.from([0x01, 0x00, 0x00, 0x00, 0x00]);
        if (num & 0x80000000) {
            num = -(buf.readUIntBE(0, 5) - num);
        }
        return num;
    }

    fetchInt64U() {
        let num = Infinity
        if (this.index + 8 <= this.size) {
            num = parseInt(
                this.params[this.index + 7] +
                this.params[this.index + 6] +
                this.params[this.index + 5] +
                this.params[this.index + 4] +
                this.params[this.index + 3] +
                this.params[this.index + 2] +
                this.params[this.index + 1] +
                this.params[this.index], 16);
            this.index += 8;
        } else {
            throw new MessageSizeError(`Already expect the lenght of params ${this.size}!`);
        }
        return num;
    }
};
