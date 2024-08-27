import { Protocol } from './Protocol.js';

export default class SerialInterface {

    constructor() {
        this.buffer = ""
        this.listOfIncommingMessages = [];
    }

    async init() {
        if ("serial" in navigator) {
            const filters = [
                { usbVendorId: 0x10C4, usbProductId: 0xEA60 },
            ];
            this.port = await navigator.serial.requestPort({ filters });
            try {
                await this.port.open({ baudRate: 115200 });

                const textEncoder = new TextEncoderStream();
                this.writableStreamClosed = textEncoder.readable.pipeTo(this.port.writable);
                this.writer = textEncoder.writable.getWriter();

                const textDecoder = new TextDecoderStream();
                this.readableStreamClosed = this.port.readable.pipeTo(textDecoder.writable);
                this.reader = textDecoder.readable.getReader();
                this.listOfIncommingMessages = []
                this.threadRx();

            } catch (error) {
                alert(error)
            }
        }
    }

    async close() {
        this.listOfIncommingMessages = []
        this.reader.cancel();
        await this.readableStreamClosed.catch(() => { });
        this.writer.close();
        await this.writableStreamClosed;
        await this.port.close();
    }

    async write(data) {
        await this.writer.write(data);
    }

    read() {
        return this.listOfIncommingMessages[0];
    }

    async console(callbackConsole) {
        this.callbackConsole = callbackConsole;
    }

    MessageAvailable() {
        return this.listOfIncommingMessages.length;
    }

    PopMessage() {
        return this.listOfIncommingMessages.pop();
    }

    async threadRx() {
        while (this.port.readable) {
            try {
                while (true) {
                    const { value, done } = await this.reader.read();
                    if (done) {
                        this.reader.releaseLock();
                        break;
                    }
                    if (value) {
                        this.buffer += value.replace('\n', '');
                        if (this.buffer.length > 0 && this.buffer.indexOf('>') != -1) {
                            if (this.buffer.indexOf('<') == 0) {
                                if (this.buffer.indexOf('<') != -1) {
                                    const raw = this.buffer.substring(this.buffer.indexOf('<'), this.buffer.indexOf('>') + 1);
                                    if (raw.length > 0) {
                                        try {
                                            let frame = Protocol.decode(raw);
                                            this.listOfIncommingMessages.push(frame);
                                        }
                                        catch (msg) {
                                            window.console.error("Decoding error: " + raw + "\n" + msg);
                                        }
                                        this.buffer = this.buffer.substring(raw.length);
                                    }
                                }
                            }
                            else {
                                this.buffer = this.buffer.substring(this.buffer.indexOf('<'));
                            }
                        }
                    }
                }
            } catch (error) {
            }
        }
    }
};