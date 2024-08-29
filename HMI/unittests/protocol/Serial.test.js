import { jest } from '@jest/globals';
import SerialInterface from '../../js/protocol/Serial.js';

class SerialPortMock {
    constructor() {
        this.readable = true;
        this.readable = new ReadableStream();
        this.writable = new WritableStream();
    }

    async open() {
        return Promise.resolve();
    }

    async close() {
        return Promise.resolve();
    }
}

describe('SerialInterface', () => {
    let serialInterface;
    let mockSerial;

    beforeEach(() => {
        mockSerial = {
            serial: {
                requestPort: jest.fn().mockResolvedValue(new SerialPortMock())
            }
        }
        SerialInterface.init = false;
        serialInterface = new SerialInterface();
    });

    it('should initialize the serial port correctly', async () => {
        serialInterface.threadRx = jest.fn();
        await serialInterface.init(mockSerial);

        expect(SerialInterface.init).toEqual(true);
        expect(serialInterface.port).not.toBeNull();
        expect(serialInterface.writer).not.toBeNull();
        expect(serialInterface.reader).not.toBeNull();
        expect(serialInterface.listOfIncommingMessages).toEqual([]);
        expect(serialInterface.threadRx).toHaveBeenCalled();
    });

    it('should not initialize twice', async () => {
        await serialInterface.init(mockSerial);

        expect(SerialInterface.init).toEqual(true);
        expect(serialInterface.port).not.toBeNull();
        expect(serialInterface.writer).not.toBeNull();
        expect(serialInterface.reader).not.toBeNull();
        expect(serialInterface.listOfIncommingMessages).toEqual([]);

        console.error = jest.fn();
        await serialInterface.init(mockSerial);
        expect(SerialInterface.init).toEqual(true);
        expect(console.error).toHaveBeenCalledWith('Already open or Web Serial API not supported in this browser.');
    });

    it('should close the serial port correctly', async () => {
        await serialInterface.init(mockSerial);
        await serialInterface.close();
        expect(serialInterface.listOfIncommingMessages).toEqual([]);
        expect(SerialInterface.init).toEqual(false);
    });

    it('should handle initialization error', async () => {
        mockSerial.serial.requestPort.mockRejectedValueOnce(new Error('Test Error'));
        console.error = jest.fn();

        await serialInterface.init(mockSerial);
        expect(SerialInterface.init).toEqual(false);
        expect(console.error).toHaveBeenCalledWith('Initialization error:', expect.any(Error));
    });

    it('should log error if Web Serial API is not supported', async () => {
        console.error = jest.fn();
        await serialInterface.init({});
        expect(SerialInterface.init).toEqual(false);
        expect(console.error).toHaveBeenCalledWith('Already open or Web Serial API not supported in this browser.');
    });

    test('should write data', async () => {
        serialInterface.writer = { write: jest.fn() };
        await serialInterface.write('test data');
        expect(serialInterface.writer.write).toHaveBeenCalledWith('test data');
    });

    test('should not write data', async () => {
        await serialInterface.write('test data');
        expect(console.error).toHaveBeenCalledWith('Serial not initialized.');
    });

    test('should read first message', () => {
        serialInterface.listOfIncommingMessages.push('message1', 'message2');
        expect(serialInterface.read()).toBe('message1');
    });

    test('should return null if no messages are available', () => {
        expect(serialInterface.read()).toBeNull();
    });

    test('should indicate if messages are available', () => {
        expect(serialInterface.messageAvailable()).toBe(false);
        serialInterface.listOfIncommingMessages.push('message1');
        expect(serialInterface.messageAvailable()).toBe(true);
    });

    test('should pop the last message', () => {
        serialInterface.listOfIncommingMessages.push('message1', 'message2');
        expect(serialInterface.popMessage()).toBe('message2');
        expect(serialInterface.listOfIncommingMessages).toEqual(['message1']);
    });

    test('should handle incoming messages', () => {
        serialInterface.catchIncommingMessage('<000000>');
        expect(serialInterface.listOfIncommingMessages).toEqual([
            {
                "cluster": { "code": "00", "name": "GENERAL", },
                "command": { "code": "00", "name": "VERSION", },
                "index": 0,
                "params": undefined,
                "raw": "<000000>",
                "size": 0,
            },
        ]);
    });

    test('should handle many incoming messages', () => {
        serialInterface.catchIncommingMessage('<000000><000000><000000><000000><000000><000000>');

        expect(serialInterface.buffer.length).toEqual(0);
        expect(serialInterface.listOfIncommingMessages.length).toEqual(6);
    });

    test('should handle many incoming messages with truncated start', () => {
        serialInterface.catchIncommingMessage('0000><000000><000000><000000><000000><000000>');

        expect(serialInterface.buffer.length).toEqual(0);
        expect(serialInterface.listOfIncommingMessages.length).toEqual(5);
    });

    test('should handle one incoming truncated messages', () => {
        serialInterface.catchIncommingMessage('000><');

        expect(serialInterface.buffer.length).toEqual('<'.length);
        expect(serialInterface.listOfIncommingMessages.length).toEqual(0);
    });
    test('should handle one incoming other truncated messages', () => {
        serialInterface.catchIncommingMessage('><000');

        expect(serialInterface.buffer.length).toEqual('<000'.length);
        expect(serialInterface.listOfIncommingMessages.length).toEqual(0);
    });

    test('should handle many incoming messages with truncated end', () => {
        serialInterface.catchIncommingMessage('<000000><000000><000000><000000><000000><000');

        expect(serialInterface.buffer.length).toEqual('<000'.length);
        expect(serialInterface.listOfIncommingMessages.length).toEqual(5);
    });

    test('should handle many incoming messages with truncated start and end', () => {
        serialInterface.catchIncommingMessage('0000><000000><000000><000000><000000><000');

        expect(serialInterface.buffer.length).toEqual('<000'.length);
        expect(serialInterface.listOfIncommingMessages.length).toEqual(4);
    });

    test('should handle many incoming messages with truncated start and end and malformed frame', () => {

        console.error = jest.fn();
        serialInterface.catchIncommingMessage('0000><000000><00<0>000><000000><000000><000');
        expect(console.error).toHaveBeenCalledTimes(1);
        expect(serialInterface.buffer.length).toEqual('<000'.length);
        expect(serialInterface.listOfIncommingMessages.length).toEqual(3);
    });

    test('should handle decoding errors', () => {
        serialInterface.catchIncommingMessage('<message>');
        expect(serialInterface.buffer.length).toEqual(0);
    });


});
