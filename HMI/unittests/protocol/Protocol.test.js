import {Protocol} from '../../js/protocol/Protocol.js';
import {Clusters} from '../../js/protocol/Cluster.js';

describe('Protocol', () => {

    test('should handle incoming messages from cluster IMU', () => {
        const message = Protocol.decode("Rx", '<010303010203>');

        expect(message.cluster.name).toBe('IMU');
        expect(message.cluster.code).toBe('01');
        expect(message.command.name).toBe('MAG');
        expect(message.command.code).toBe('03');
        expect(message.direction).toBe("Rx");
        expect(message.index).toBe(0);
        expect(message.timeout).toBe(0);
        expect(message.size).toBe(3);
        expect(message.params).toEqual(['01', '02', '03']);
        expect(message.raw).toBe('<010303010203>');
    });

    test('should handle decoding error if frame is incorrect size', () => {
        const throwedDecode = () => {
            Protocol.decode("Tx", '<01030301020304>');
        };
        expect(throwedDecode).toThrow(Error);
        expect(throwedDecode).toThrow("Decoding error! incorrect size: <01030301020304>");
    });

    test('should handle decoding error if malformed frame', () => {
        const throwedDecode = () => {
            Protocol.decode("Tx", '<01030301020304');
        };
        expect(throwedDecode).toThrow(Error);
        expect(throwedDecode).toThrow("Should starts and ends with \"<\" and \">\": <01030301020304");
    });
});