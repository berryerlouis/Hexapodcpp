export default class SerialInterface {

    constructor() {
        this.buffer = ""
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

                this.threadRx();

            } catch (error) {
                alert(error)
            }
        }
    }

    async close() {
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
        return this.buffer;
    }


    eat(data) {
        this.buffer = this.buffer.substring(data.length);
    }

    async console(callbackConsole) {
        this.callbackConsole = callbackConsole;
    }

    dataAvailable() {
        return this.buffer.length
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
                        this.buffer += value;
                    }
                }
            } catch (error) {
            }
        }
    }
};