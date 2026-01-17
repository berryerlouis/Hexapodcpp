import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterImuCommands {
    ALL = 'ALL',
    ACC = 'ACC',
    GYR = 'GYR',
    MAG = 'MAG',
    TMP = 'TMP',
    YAWPITCHROLL = 'YAWPITCHROLL',
    PRESSURE = 'PRESSURE',
    ALTITUDE = 'ALTITUDE',
    TMPBAR = 'TMPBAR',
    START_CALIB = 'STARTCALIB',
}

export const CommandImuId = '01';

export default class ClusterImu extends Cluster {
    constructor() {
        super(
            ClusterName.IMU,
            CommandImuId,
            [
                {code: '00', name: ClusterImuCommands.ALL},
                {code: '01', name: ClusterImuCommands.ACC},
                {code: '02', name: ClusterImuCommands.GYR},
                {code: '03', name: ClusterImuCommands.MAG},
                {code: '04', name: ClusterImuCommands.TMP},
                {code: '05', name: ClusterImuCommands.YAWPITCHROLL},
                {code: '06', name: ClusterImuCommands.PRESSURE},
                {code: '07', name: ClusterImuCommands.ALTITUDE},
                {code: '08', name: ClusterImuCommands.TMPBAR},
                {code: '09', name: ClusterImuCommands.START_CALIB},
                {code: 'FF', name: ClusterGenericCommands.GENERIC}
            ]
        );
    }
}
