import { Clusters } from './Cluster.js'
import { Protocol } from './Protocol.js'

export class Message {
    params;
    cluster;
    command;
    size;
    params;
    raw;

    constructor() { }

    build(cluster = undefined, command = undefined, size = 0, params = null) {

        this.cluster = Clusters.findClusterByName(cluster);
        this.command = Clusters.findCommandByName(command);
        this.size = size;
        this.params = params;
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
};
