import Protocol, {Direction, Encoding} from "./protocol.ts";
import {Cluster, ClusterName, Command, CommandName} from "./clusters/clusterType.ts";
import {getClusterByName, getCommandByName} from "./clusters/clusters.ts";

export class MessageSizeError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'MessageSizeError';
    }
}
export default class Message {
    date: string = '';
    direction: Direction;
    cluster: Cluster;
    command: Command;
    size: number;
    params: number[];
    encode: Encoding[];
    raw: string;
    timeout: number = 0;
    retry: number = 0;

    constructor( clusterName: ClusterName, commandName: CommandName, size :number = 0, params: (number) [] = [], encode: (Encoding) [] = []){
        this.direction = Direction.TX;
        this.cluster = getClusterByName(clusterName);
        this.command = getCommandByName(this.cluster, commandName);
        this.size = size;
        this.params = params;
        this.encode = encode;
        if(params && this.encode.length == 0) {
            for (let i = 0; i < params.length; i++) {
                this.encode[i] = 0xFF;
            }
        }
        this.raw = Protocol.encode(this.cluster, this.command, this.size, this.params, this.encode );
        this.timeout = 0;
        this.retry = 0;

        return this;
    }

    setDate() {
        let date = new Date();
        this.date = new Date(date.getTime() - date.getTimezoneOffset() * 60000).toISOString().replace('T', ' ').replace('Z', ' ');
    }

    toString(): string {
        let ret = this.date;
        ret += '\t' + this.direction;
        ret += '\t' + this.raw;
        ret += '\t' + this.cluster?.name;
        ret += '\t' + this.command?.name;
        return ret;
    }
}