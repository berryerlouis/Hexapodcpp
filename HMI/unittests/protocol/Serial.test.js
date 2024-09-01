import { jest } from '@jest/globals';
import { SerialInterface } from '../../js/protocol/Serial.js';
import { Message } from '../../js/protocol/Message.js';
import { ClusterName, CommandGeneral } from '../../js/protocol/Cluster.js'

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
        serialInterface = new SerialInterface();
    });

    it('should initialize the serial port correctly', async () => {
        serialInterface.threadRx = jest.fn();
        await serialInterface.init(mockSerial);

        expect(serialInterface.initialized).toEqual(true);
        expect(serialInterface.port).not.toBeNull();
        expect(serialInterface.writer).not.toBeNull();
        expect(serialInterface.reader).not.toBeNull();
        expect(serialInterface.listOfIncommingMessages).toEqual([]);
        expect(serialInterface.threadRx).toHaveBeenCalled();
    });

    it('should not initialize twice', async () => {
        await serialInterface.init(mockSerial);

        expect(serialInterface.initialized).toEqual(true);
        expect(serialInterface.port).not.toBeNull();
        expect(serialInterface.writer).not.toBeNull();
        expect(serialInterface.reader).not.toBeNull();
        expect(serialInterface.listOfIncommingMessages).toEqual([]);

        console.error = jest.fn();
        await serialInterface.init(mockSerial);
        expect(serialInterface.initialized).toEqual(true);
        expect(console.error).toHaveBeenCalledWith('Already open or Web Serial API not supported in this browser.');
    });

    it('should close the serial port correctly', async () => {

        serialInterface.reader = { cancel: jest.fn() };
        serialInterface.readableStreamClosed = { catch: jest.fn() };
        serialInterface.writer = { close: jest.fn() };
        serialInterface.port = { close: jest.fn() };

        await serialInterface.close();

        expect(serialInterface.listOfIncommingMessages).toEqual([]);
        expect(serialInterface.initialized).toEqual(false);
        expect(serialInterface.reader.cancel).toHaveBeenCalled();
        expect(serialInterface.readableStreamClosed.catch).toHaveBeenCalled();
        expect(serialInterface.writer.close).toHaveBeenCalled();
        expect(serialInterface.port.close).toHaveBeenCalled();
    });

    it('should close the serial port correctly before init', async () => {
        await serialInterface.close();
        expect(serialInterface.listOfIncommingMessages).toEqual([]);
        expect(serialInterface.initialized).toEqual(false);
    });

    it('should handle initialization error', async () => {
        mockSerial.serial.requestPort.mockRejectedValueOnce(new Error('Test Error'));
        console.error = jest.fn();

        await serialInterface.init(mockSerial);
        expect(serialInterface.initialized).toEqual(false);
        expect(console.error).toHaveBeenCalledWith('Initialization error:', expect.any(Error));
    });

    it('should log error if Web Serial API is not supported', async () => {
        console.error = jest.fn();
        await serialInterface.init({});
        expect(serialInterface.initialized).toEqual(false);
        expect(console.error).toHaveBeenCalledWith('Already open or Web Serial API not supported in this browser.');
    });

    test('should write data', async () => {
        serialInterface.writer = { write: jest.fn() };
        serialInterface.notifyWrite = jest.fn();
        await serialInterface.write('test data');
        expect(serialInterface.writer.write).toHaveBeenCalled();
        expect(serialInterface.notifyWrite).toHaveBeenCalled();
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

        expect(serialInterface.listOfIncommingMessages[0].cluster.name).toBe('GENERAL');
        expect(serialInterface.listOfIncommingMessages[0].cluster.code).toBe('00');
        expect(serialInterface.listOfIncommingMessages[0].command.name).toBe('VERSION');
        expect(serialInterface.listOfIncommingMessages[0].command.code).toBe('00');
        expect(serialInterface.listOfIncommingMessages[0].direction).toBe("Rx");
        expect(serialInterface.listOfIncommingMessages[0].index).toBe(0);
        expect(serialInterface.listOfIncommingMessages[0].timeout).toBe(0);
        expect(serialInterface.listOfIncommingMessages[0].size).toBe(0);
        expect(serialInterface.listOfIncommingMessages[0].params).toEqual(null);
        expect(serialInterface.listOfIncommingMessages[0].raw).toBe('<000000>');
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

    test('should add callback Read', () => {
        serialInterface.addCallbackRead(() => { });
        expect(serialInterface.listOfCallbackRead.length).toEqual(1);
    });

    test('should add callback Write', () => {
        serialInterface.addCallbackWrite(() => { });
        expect(serialInterface.listOfCallbackWrite.length).toEqual(1);
    });

    test('should call notify Read', () => {
        let cbRead = jest.fn();
        serialInterface.addCallbackRead(cbRead);
        expect(serialInterface.listOfCallbackRead.length).toEqual(1);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        serialInterface.notifyRead(msg);
        expect(cbRead).toHaveBeenCalledWith(msg);
    });

    test('should call notify Write', () => {
        let cbWrite = jest.fn();
        serialInterface.addCallbackWrite(cbWrite);
        expect(serialInterface.listOfCallbackWrite.length).toEqual(1);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        serialInterface.notifyWrite(msg);
        expect(cbWrite).toHaveBeenCalledWith(msg);
    });
});
