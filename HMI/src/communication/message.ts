import Protocol, { Direction, Encoding } from "./protocol.ts";
import { Cluster, ClusterName, Command, CommandName } from "./clusters/clusterType.ts";
import { getClusterByName, getCommandByName } from "./clusters/clusters.ts";

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

    constructor(clusterName: ClusterName, commandName: CommandName, params: (number)[] = [], encode: (Encoding)[] = []) {
        this.direction = Direction.TX;
        this.cluster = getClusterByName(clusterName);
        this.command = getCommandByName(this.cluster, commandName);
        this.size = params.length ? params.length : 0;
        this.params = params;
        this.encode = encode;
        if (params && this.encode.length === 0) {
            for (let i = 0; i < params.length; i++) {
                this.encode[i] = 0xFF;
            }
        }
        this.raw = Protocol.encode(this.cluster, this.command, this.size, this.params, this.encode);
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

    getValueBool(index: number) {
        if (!this.params || index >= this.params.length) {
            throw new MessageSizeError(`No value at index ${index} in params`);
        }
        return this.params[index] === 1;
    }

    getValueUint8(index: number) {
        if (!this.params || index >= this.params.length) {
            throw new MessageSizeError(`No value at index ${index} in params`);
        }
        return this.params[index] & 0xFF;
    }

    getValueInt8(index: number) {
        let num = this.getValueUint8(index);
        if (num & 0x80) {
            num = -(0x100 - num);
        }
        return num;
    }

    getValueUint16(index: number) {
        if (!this.params || index + 1 >= this.params.length) {
            throw new MessageSizeError(`No value at index ${index} in params`);
        }
        return (this.params[index + 1] << 8) + this.params[index];
    }

    getValueInt16(index: number) {
        let num = this.getValueUint16(index);
        if (num & 0x8000) {
            num = -(0x10000 - num);
        }
        return num;
    }

    getValueUint24(index: number) {
        if (!this.params || index + 2 >= this.params.length) {
            throw new MessageSizeError(`No value at index ${index} in params`);
        }
        return (this.params[index + 2] << 16) + (this.params[index + 1] << 8) + this.params[index];
    }

    getValueInt24(index: number) {
        let num = this.getValueUint24(index);
        if (num & 0x800000) {
            num = -(0x1000000 - num);
        }
        return num;
    }

    getValueUint32(index: number) {
        if (!this.params || index + 3 >= this.params.length) {
            throw new MessageSizeError(`No value at index ${index} in params`);
        }
        return (this.params[index + 3] << 24) + (this.params[index + 2] << 16) + (this.params[index + 1] << 8) + this.params[index];
    }

    getValueInt32(index: number) {
        let num = this.getValueUint32(index);
        if (num & 0x80000000) {
            num = -(0x100000000 - num);
        }
        return num;
    }
}