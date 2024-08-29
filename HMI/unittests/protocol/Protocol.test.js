import { jest } from '@jest/globals';
import { Protocol } from '../../js/protocol/Protocol.js';
import { Clusters, Cluster, Command, ClusterName, CommandGeneral, CommandImu, CommandProximity, CommandServo } from '../../js/protocol/Cluster.js';

describe('Protocol', () => {

    test('should handle incoming messages from cluster IMU', () => {
        expect(Protocol.decode('<010303010203>')).toEqual(
            {
                "cluster":
                {
                    "code": "01",
                    "name": "IMU",
                },
                "command":
                {
                    "code": "03",
                    "name": "MAG",
                },
                "index": 0,
                "params": ['01', '02', '03'],
                "raw": "<010303010203>",
                "size": 3,
            },
        );
    });

    test('should handle decoding error if frame is incorrect size', () => {
        const throwedDecode = () => {
            Protocol.decode('<01030301020304>');
        };
        expect(throwedDecode).toThrow(Error);
        expect(throwedDecode).toThrow("Decoding error! incorect size: <01030301020304>");
    });

    test('should handle decoding error if malformed frame', () => {
        const throwedDecode = () => {
            Protocol.decode('<01030301020304');
        };
        expect(throwedDecode).toThrow(Error);
        expect(throwedDecode).toThrow("Should starts and ends with \"<\" and \">\": <01030301020304");
    });
});