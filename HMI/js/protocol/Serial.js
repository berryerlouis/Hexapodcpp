import {Protocol} from './Protocol.js';

export class SerialInterface {
    constructor() {
        this.initialized = false;
        this.buffer = "";
        this.listOfIncommingMessages = [];
        this.listOfCallbackRead = [];
        this.listOfCallbackWrite = [];
    }

    getScriptPath(foo) {
        return window.URL.createObjectURL(new Blob([foo.toString().match(/^\s*function\s*\(\s*\)\s*\{(([\s\S](?!\}$))*[\s\S])/)[1]], {type: 'text/javascript'}));
    }

    async init(nav) {
        if ("serial" in nav && this.initialized === false) {
            const filters = [{usbVendorId: 0x10C4, usbProductId: 0xEA60}];
            try {
                this.port = await nav.serial.requestPort({filters});
                await this.port.open({baudRate: 115200});

                const textEncoder = new TextEncoderStream();
                this.writableStreamClosed = textEncoder.readable.pipeTo(this.port.writable);
                this.writer = textEncoder.writable.getWriter();

                const textDecoder = new TextDecoderStream();
                this.readableStreamClosed = this.port.readable.pipeTo(textDecoder.writable);
                this.reader = textDecoder.readable.getReader();

                this.listOfIncommingMessages = [];
                this.buffer = "";
                this.initialized = true;
                await this.threadRx();

            } catch (error) {
                console.error("Initialization error:", error);
            }
        } else {
            console.error("Already open or Web Serial API not supported in this browser.");
        }
    }

    async close() {
        this.initialized = false;
        this.listOfIncommingMessages = [];
        if (this.reader) {
            await this.reader.cancel();
            await this.readableStreamClosed.catch(() => {
            });
        }
        if (this.writer) {
            await this.writer.close();
            await this.writableStreamClosed;
        }
        if (this.port) {
            await this.port.close();
        }
    }

    async write(message) {
        if (this.writer) {
            await this.writer.write(message.raw);
            this.notifyWrite(message);
            return true;
        } else {
            console.error("Serial not initialized.");
            return false;
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

    addCallbackRead(cb) {
        this.listOfCallbackRead.push(cb);
    }

    addCallbackWrite(cb) {
        this.listOfCallbackWrite.push(cb);
    }

    notifyRead(message) {
        this.listOfCallbackRead.forEach(function (cb) {
            cb(message);
        });
    }

    notifyWrite(message) {
        this.listOfCallbackWrite.forEach(function (cb) {
            cb(message);
        });
    }

    catchIncomingMessage(value) {
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
                    let frame = Protocol.decode("Rx", raw);
                    this.listOfIncommingMessages.push(frame);
                    this.notifyRead(frame);
                } catch (msg) {
                    console.error(`Decoding error: "${raw}"\n"${msg}"`);
                }
                this.buffer = this.buffer.substring(endIndex);
            }
        }
    }

    async threadRx() {
        try {
            setInterval(async () => {
                if (this.port.readable) {
                    const {value, done} = await this.reader.read();
                    if (done) {
                        this.reader.releaseLock();
                    }
                    if (value) {
                        this.catchIncomingMessage(value.replace('\n', ''));
                    }
                }
            }, 0);

        } catch (error) {
            await this.close()
            console.error(`Thread Serial error!"`);
        }
    }
}
