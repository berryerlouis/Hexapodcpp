export class MessageSizeError extends Error {
    constructor(message) {
        super(message);
        this.name = 'MessageSizeError';
    }
}