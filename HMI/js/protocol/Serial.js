import { Protocol } from './Protocol.js';

export default class SerialInterface {
    static init = false;
    constructor() {
        this.buffer = "";
        this.listOfIncommingMessages = [];
    }

    async init(nav) {
        if ("serial" in nav && SerialInterface.init == false) {
            const filters = [{ usbVendorId: 0x10C4, usbProductId: 0xEA60 }];
            try {
                this.port = await nav.serial.requestPort({ filters });
                await this.port.open({ baudRate: 115200 });

                const textEncoder = new TextEncoderStream();
                this.writableStreamClosed = textEncoder.readable.pipeTo(this.port.writable);
                this.writer = textEncoder.writable.getWriter();

                const textDecoder = new TextDecoderStream();
                this.readableStreamClosed = this.port.readable.pipeTo(textDecoder.writable);
                this.reader = textDecoder.readable.getReader();

                this.listOfIncommingMessages = [];
                this.buffer = "";
                this.threadRx();
                SerialInterface.init = true;
            } catch (error) {
                console.error("Initialization error:", error);
            }
        } else {
            console.error("Already open or Web Serial API not supported in this browser.");
        }
    }

    async close() {
        this.listOfIncommingMessages = [];
        if (this.reader) {
            await this.reader.cancel();
            await this.readableStreamClosed.catch(() => { });
        }
        if (this.writer) {
            await this.writer.close();
            await this.writableStreamClosed;
        }
        if (this.port) {
            await this.port.close();
            SerialInterface.init = false;
        }
    }

    async write(data) {
        if (this.writer) {
            await this.writer.write(data);
        } else {
            console.error("Serial not initialized.");
        }
    }

    read() {
        return this.listOfIncommingMessages.length > 0 ? this.listOfIncommingMessages[0] : null;
    }

    messageAvailable() {
        return this.listOfIncommingMessages.length > 0;
    }

    popMessage() {
        return this.listOfIncommingMessages.pop();
    }

    catchIncommingMessage(value) {
        this.buffer += value;

        while (this.buffer.length > 0 && this.buffer.includes('>')) {
            const startIndex = this.buffer.indexOf('<');
            const endIndex = this.buffer.indexOf('>') + 1;

            if (startIndex > 0) {
                this.buffer = this.buffer.substring(startIndex);
                continue;
            }

            const raw = this.buffer.substring(startIndex, endIndex);

            if (raw.length > 0) {
                try {
                    let frame = Protocol.decode(raw);
                    this.listOfIncommingMessages.push(frame);
                } catch (msg) {
                    console.error(`Decoding error: "${raw}"\n"${msg}"`);
                }
                this.buffer = this.buffer.substring(endIndex);
            }
        }
    }

    async threadRx() {
        try {
            while (this.port.readable) {
                const { value, done } = await this.reader.read();
                if (done) {
                    this.reader.releaseLock();
                    break;
                }
                if (value) {
                    this.catchIncommingMessage(value.replace('\n', ''));
                }
            }
        } catch (error) {
            console.error(`Thread Serial error!"`);
        }
    }
}
