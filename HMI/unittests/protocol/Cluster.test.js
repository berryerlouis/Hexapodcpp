import { ClusterNotFoundError, CommandNotFoundError } from '../../js/protocol/ClusterError.js';
import { Clusters, Cluster, Command, ClusterName, CommandGeneral, CommandImu, CommandProximity, CommandServo } from '../../js/protocol/Cluster.js';

describe('Clusters Class', () => {
    test('should find cluster by name or code', () => {
        const clusterByName = Clusters.getClusterByNameOrCode('GENERAL');
        expect(clusterByName.name).toBe('GENERAL');

        const clusterByCode = Clusters.getClusterByNameOrCode('00', true);
        expect(clusterByCode.code).toBe('00');
    });

    test('should throw error if cluster by name or code not found', () => {
        expect(() => {
            Clusters.getClusterByNameOrCode('NON_EXISTENT_CLUSTER');
        }).toThrow(ClusterNotFoundError);

        expect(() => {
            Clusters.getClusterByNameOrCode('99', true);
        }).toThrow(ClusterNotFoundError);
    });

    test('should get cluster by name', () => {
        const cluster = Clusters.getClusterByName('GENERAL');
        expect(cluster).toEqual({ code: '00', name: 'GENERAL' });
    });

    test('should get cluster by code', () => {
        const cluster = Clusters.getClusterByCode('00');
        expect(cluster).toEqual({ code: '00', name: 'GENERAL' });
    });

    test('should get command by name', () => {
        const command = Clusters.getCommandByName({ code: '00', name: 'GENERAL' }, 'VERSION');
        expect(command.name).toBe('VERSION');
        expect(command.code).toBe('00');
    });

    test('should throw error if command by name not found', () => {
        expect(() => {
            Clusters.getCommandByName({ code: '00', name: 'GENERAL' }, 'NON_EXISTENT_COMMAND');
        }).toThrow(CommandNotFoundError);
    });

    test('should get command by code', () => {
        const command = Clusters.getCommandByCode({ code: '00', name: 'GENERAL' }, '00');
        expect(command.name).toBe('VERSION');
        expect(command.code).toBe('00');
    });

    test('should throw error if command by code not found', () => {
        expect(() => {
            Clusters.getCommandByCode({ code: '00', name: 'GENERAL' }, '99');
        }).toThrow(CommandNotFoundError);
    });
});
