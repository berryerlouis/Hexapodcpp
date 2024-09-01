import { Message } from '../../js/protocol/Message';
import { ClusterNotFoundError, CommandNotFoundError } from '../../js/protocol/ClusterError.js';
import { MessageSizeError } from '../../js/protocol/MessageError.js';
import { ClusterName, CommandGeneral } from '../../js/protocol/Cluster';

describe('Message', () => {
    let message;

    beforeEach(() => {
        message = new Message();
    });

    test('should build message correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION);
        expect(message.cluster.name).toBe('GENERAL');
        expect(message.cluster.code).toBe('00');
        expect(message.command.name).toBe('VERSION');
        expect(message.command.code).toBe('00');
        expect(message.size).toBe(0);
        expect(message.params).toEqual(null);
        expect(message.raw).toBe('<000000>');
    });

    test('should build message with params correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['01', '02', '03', '04']);
        expect(message.cluster.name).toBe('GENERAL');
        expect(message.cluster.code).toBe('00');
        expect(message.command.name).toBe('VERSION');
        expect(message.command.code).toBe('00');
        expect(message.size).toBe(4);
        expect(message.params).toEqual(['01', '02', '03', '04']);
        expect(message.raw).toBe('<00000401020304>');
    });

    test('should not build message with wrong cluster name', () => {
        const throwedBuild = () => {
            message.build("CLUSTER_WRONG", CommandGeneral.VERSION);
        };
        expect(throwedBuild).toThrow(ClusterNotFoundError);
    });

    test('should not build message with wrong command name', () => {
        const throwedBuild = () => {
            message.build("Rx", ClusterName.GENERAL, "COMMAND_WRONG");
        };
        expect(throwedBuild).toThrow(CommandNotFoundError);
    });

    test('should not build message with wrong size', () => {
        const throwedBuild = () => {
            message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 1, ['01', '02', '03', '04']);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
    });

    test('should not build message with no size set', () => {
        const throwedBuild = () => {
            message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 0, ['01', '02', '03', '04']);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
    });

    test('should not build message with null param', () => {
        const throwedBuild = () => {
            message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, null);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
    });

    test('should display Rx message as string using toString', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['01', '02', '03', '04']);
        expect(message.cluster.name).toBe('GENERAL');
        expect(message.cluster.code).toBe('00');
        expect(message.command.name).toBe('VERSION');
        expect(message.command.code).toBe('00');
        expect(message.size).toBe(4);
        expect(message.params).toEqual(['01', '02', '03', '04']);
        expect(message.raw).toBe('<00000401020304>');
        const expectedSubstring = "Rx      <00000401020304>        GENERAL VERSION";
        const pattern = new RegExp(`.*${expectedSubstring.replace(/\s+/g, '\\s+')}`);
        expect(message.toString("Rx")).toMatch(pattern);
    });

    test('should display Tx message as string using toString', () => {
        message.build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['01', '02', '03', '04']);
        expect(message.cluster.name).toBe('GENERAL');
        expect(message.cluster.code).toBe('00');
        expect(message.command.name).toBe('VERSION');
        expect(message.command.code).toBe('00');
        expect(message.size).toBe(4);
        expect(message.params).toEqual(['01', '02', '03', '04']);
        expect(message.raw).toBe('<00000401020304>');
        const expectedSubstring = "Tx      <00000401020304>        GENERAL VERSION";
        const pattern = new RegExp(`.*${expectedSubstring.replace(/\s+/g, '\\s+')}`);
        expect(message.toString("Tx")).toMatch(pattern);
    });

    test('should fetchInt8U correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 5, ['01', '02', '03', '04', 'FF']);
        expect(message.fetchInt8U()).toBe(1);
        expect(message.fetchInt8U()).toBe(2);
        expect(message.fetchInt8U()).toBe(3);
        expect(message.fetchInt8U()).toBe(4);
        expect(message.fetchInt8U()).toBe(255);
    });

    test('Try to fetch more than expected', () => {

        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION);
        let throwedBuild = () => {
            expect(message.fetchInt8U()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
        throwedBuild = () => {
            expect(message.fetchInt8S()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
        throwedBuild = () => {
            expect(message.fetchInt16U()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
        throwedBuild = () => {
            expect(message.fetchInt16S()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
        throwedBuild = () => {
            expect(message.fetchInt24U()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
        throwedBuild = () => {
            expect(message.fetchInt32U()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
        throwedBuild = () => {
            expect(message.fetchInt32S()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
        throwedBuild = () => {
            expect(message.fetchInt64U()).toBe(1);
        };
        expect(throwedBuild).toThrow(MessageSizeError);
    });

    test('should fetchInt8S negative number correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['81', '02', '03', '04']);
        expect(message.fetchInt8S()).toBe(-127);
    });

    test('should fetchInt8S positive number correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['7F', '02', '03', '04']);
        expect(message.fetchInt8S()).toBe(127);
    });

    test('should fetchInt16U correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['01', '02', '03', '04']);
        expect(message.fetchInt16U()).toBe(513); // 0x0201
    });

    test('should fetchInt16S negative number correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['2F', 'E9', '03', '04']);
        expect(message.fetchInt16S()).toBe(-5841);
    });

    test('should fetchInt16S positive number correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['D1', '16', '03', '04']);
        expect(message.fetchInt16S()).toBe(5841);
    });

    test('should fetchInt24U correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['01', '02', '03', '04']);
        expect(message.fetchInt24U()).toBe(197121); // 0x030201
    });

    test('should fetchInt32U correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['01', '02', '03', '04']);
        expect(message.fetchInt32U()).toBe(67305985); // 0x04030201
    });

    test('should fetchInt32S negative number correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['EB', 'DC', 'A6', 'FF']);
        expect(message.fetchInt32S()).toBe(-5841685);
    });

    test('should fetchInt32S positive number correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 4, ['87', 'D6', '12', '00']);
        expect(message.fetchInt32S()).toBe(1234567);
    });

    test('should fetchInt64U correctly', () => {
        message.build("Rx", ClusterName.GENERAL, CommandGeneral.VERSION, 8, ['01', '02', '03', '04', '05', '06', '07', '08']);
        expect(message.fetchInt64U()).toBe(578437695752307201); // 0x0807060504030201
    });

});
