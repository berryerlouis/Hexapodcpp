export class MessageSizeError extends Error {
    constructor(message: string) {
        super(message);
        this.name = 'MessageSizeError';
    }
}