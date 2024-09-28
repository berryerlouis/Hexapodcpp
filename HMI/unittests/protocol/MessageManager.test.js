import {jest} from '@jest/globals';
import {Message} from '../../js/protocol/Message.js';
import {MessageManager} from '../../js/protocol/MessageManager.js';
import {ClusterName, CommandGeneral} from '../../js/protocol/Cluster.js'


describe('MessageManager', () => {
    let messageManager;
    let mockSerialInterface;

    beforeEach(() => {
        mockSerialInterface = {
            initialized: false,
            addCallbackRead() {
            }
        }
        messageManager = new MessageManager(mockSerialInterface);
        expect(messageManager.listMessagesToSent).toEqual([]);
        expect(messageManager.listOfCallbackRead).toEqual([]);
        expect(messageManager.listOfCallbackWrite).toEqual([]);
        expect(messageManager.listOfCallbackTimeout).toEqual([]);
    });

    it('should write a new message if Serial is not initalized correctly', async () => {
        mockSerialInterface.initialized = false;
        messageManager.write("message", (message) => {
        });
        expect(messageManager.listMessagesToSent.length).toEqual(1);
    });

    it('should write a new message correctly', async () => {
        mockSerialInterface.initialized = true;
        messageManager.write("message", (message) => {
        });
        expect(messageManager.listMessagesToSent.length).toEqual(1);
    });

    it('should read a new message correctly if no current message', async () => {
        mockSerialInterface.initialized = true;
        messageManager.currentMessagesToSent = null;
        messageManager.notifyOnSpecificCommand = jest.fn();
        let msgRx = new Message().build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 2, ["00", "01"]);
        messageManager.read(msgRx);
        expect(messageManager.currentMessagesToSent).toEqual(null);
        expect(messageManager.notifyOnSpecificCommand).toHaveBeenCalledWith(msgRx);
    });

    it('should read a new message correctly', async () => {
        mockSerialInterface.initialized = true;
        let msgTx = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        let msgRx = new Message().build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 2, ["00", "01"]);
        messageManager.currentMessagesToSent = {
            message: msgTx, cbResponse: (message) => {
            }
        };
        messageManager.read(msgRx);
        expect(messageManager.currentMessagesToSent).toEqual(null);
    });

    test('should add callback Specific Command', () => {
        messageManager.addCallbackNotifyOnSpecificCommand("", "", () => {
        });
        expect(messageManager.listOfCallbackNotifyOnSpecificCommand.length).toEqual(1);
    });

    test('should add callback Read', () => {
        messageManager.addCallbackRead(() => {
        });
        expect(messageManager.listOfCallbackRead.length).toEqual(1);
    });

    test('should add callback Write', () => {
        messageManager.addCallbackWrite(() => {
        });
        expect(messageManager.listOfCallbackWrite.length).toEqual(1);
    });

    test('should add callback Write Timeout', () => {
        messageManager.addCallbackWriteTimeout(() => {
        });
        expect(messageManager.listOfCallbackTimeout.length).toEqual(1);
    });

    test('should call notify Read', () => {
        let cbRead = jest.fn();
        messageManager.addCallbackRead(cbRead);
        expect(messageManager.listOfCallbackRead.length).toEqual(1);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        messageManager.notifyRead(msg);
        expect(cbRead).toHaveBeenCalledWith(msg);
    });

    test('should call notify Write', () => {
        let cbWrite = jest.fn();
        messageManager.addCallbackWrite(cbWrite);
        expect(messageManager.listOfCallbackWrite.length).toEqual(1);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        messageManager.notifyWrite({message: msg});
        expect(cbWrite).toHaveBeenCalledWith(msg);
    });

    test('should call notify Write Timeout', () => {
        let cbWriteTimeout = jest.fn();
        messageManager.addCallbackWriteTimeout(cbWriteTimeout);
        expect(messageManager.listOfCallbackTimeout.length).toEqual(1);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        messageManager.notifyWriteTimeout({message: msg});
        expect(cbWriteTimeout).toHaveBeenCalledWith(msg);
    });

    test('should call notify Specific Command', () => {
        let cbSpecific = jest.fn();
        messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.VERSION, cbSpecific);
        expect(messageManager.listOfCallbackNotifyOnSpecificCommand.length).toEqual(1);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        messageManager.notifyOnSpecificCommand(msg);
        expect(cbSpecific).toHaveBeenCalledWith(msg);
    });

    test('should no call notify Specific Command', () => {
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        messageManager.notifyOnSpecificCommand(msg);
    });

    test('should no call notify Specific Command with other cluster name', () => {
        let cbSpecific = jest.fn();
        messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.VERSION, cbSpecific);
        expect(messageManager.listOfCallbackNotifyOnSpecificCommand.length).toEqual(1);
        let msg = new Message();
        msg.cluster = {name: ClusterName};
        msg.command = {name: CommandGeneral.VERSION};
        messageManager.notifyOnSpecificCommand(msg);
        expect(cbSpecific).not.toHaveBeenCalledWith(msg);
    });

    test('should update with message in queue', () => {
        mockSerialInterface.initialized = true;
        mockSerialInterface.write = jest.fn();
        mockSerialInterface.write.mockReturnValue(true);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        messageManager.write(msg);
        messageManager.update();
        expect(mockSerialInterface.write.mock.calls).toHaveLength(1);
        expect(messageManager.currentMessagesToSent.message).toEqual(msg);
    });

    test('should update with message in queue but cant write', () => {
        mockSerialInterface.initialized = true;
        mockSerialInterface.write = jest.fn();
        mockSerialInterface.write.mockReturnValue(false);
        let msg = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        messageManager.write(msg);
        messageManager.update();
        expect(mockSerialInterface.write.mock.calls).toHaveLength(1);
        expect(messageManager.currentMessagesToSent).toEqual(null);
    });

    test('should timeout a new message does not found', async () => {
        mockSerialInterface.initialized = true;
        let msgTx = new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION);
        console.warn = jest.fn();
        messageManager.currentMessagesToSent = {
            message: msgTx, cbResponse: (message) => {
            }
        };
        for (let i = 0; i < 9; i++) {
            messageManager.update();
            expect(messageManager.currentMessagesToSent.message.timeout).toEqual(i + 1);
        }
        messageManager.update();
        expect(console.warn).toHaveBeenCalledTimes(1);
        expect(messageManager.currentMessagesToSent.message.retry).toEqual(1);
        expect(messageManager.currentMessagesToSent).not.toEqual(null);
    });

    test('should update with message in queue if Serial is not initialized correctly', () => {
        mockSerialInterface.initialized = false;
        messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));
        messageManager.update();
    });

    test('should update without message in queue', () => {
        mockSerialInterface.initialized = true;
        messageManager.update();
    });

    test('should update without message in queue if Serial is not initialized correctly', () => {
        mockSerialInterface.initialized = false;
        messageManager.update();
    });
});
