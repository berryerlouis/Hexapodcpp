import { Clusters } from './Cluster.js'
import { Protocol } from './Protocol.js'

export class Message {
    params;
    cluster;
    command;
    size;
    params;
    raw;
    index;

    constructor() {
        this.index = 0;
    }

    build(cluster = undefined, command = undefined, size = 0, params = null) {

        this.cluster = Clusters.findClusterByName(cluster);
        this.command = Clusters.findCommandByName(command);
        this.size = size;
        this.params = params;
        this.index = 0;
        this.raw = Protocol.encode(this.cluster, this.command, this.size, this.params);
        return this;
    }

    toString(direction) {
        let date = new Date();
        return new Date(date.toString().split('GMT')[0] + ' UTC').toISOString().replace('T', ' ').replace('000Z', date.getMilliseconds().toString().padStart(3, '0')) +
            '\t' + direction +
            '\t' + this.raw +
            '\t' + this.cluster.name +
            '\t' + this.command.name;
    }

    fetchInt8U() {
        let num = Infinity
        if (this.index + 1 <= this.size) {
            num = parseInt(this.params[this.index], 16);
            this.index++;
        }
        return num;
    }

    fetchInt8S() {
        let num = this.fetchInt8U();
        if (num & 0x80) { num = -(0x100 - num); }
        return num;
    }

    fetchInt16U() {
        let num = Infinity
        if (this.index + 2 <= this.size) {
            num = parseInt(
                this.params[this.index + 1] +
                this.params[this.index], 16);
            this.index += 2;
        }
        return num;
    }

    fetchInt16S() {
        let num = this.fetchInt16U();
        if (num & 0x8000) { num = -(0x10000 - num); }
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
        }
        return num;
    }

    fetchInt32S() {
        let num = this.fetchInt32U();
        if (num & 0x800000) { num = -(0x1000000 - num); }
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
        }
        return num;
    }
};
