export class ClusterNotFoundError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'ClusterNotFoundError';
    }
}

export class CommandNotFoundError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'CommandNotFoundError';
    }
}