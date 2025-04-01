import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterBatteryCommands {
    STATUS = 'STATUS',
    VOLTAGE = 'VOLTAGE',
    CURRENT = 'CURRENT',
}

export const CommandBatteryId = '04';

export default class ClusterBattery extends Cluster {
    constructor() {
        super(
            ClusterName.BATTERY,
            CommandBatteryId,
            [
                { code: '00', name: ClusterBatteryCommands.STATUS },
                { code: '01', name: ClusterBatteryCommands.VOLTAGE},
                { code: '02', name: ClusterBatteryCommands.CURRENT},
                { code: 'FF', name: ClusterGenericCommands.GENERIC }
            ]
        );
    }
}
