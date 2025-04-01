import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterServoCommands {
    GET_ALL= 'GET_ALL',
    GET_ANGLE= 'GET_ANGLE',
    SET_ANGLE= 'SET_ANGLE',
    GET_MIN= 'GET_MIN',
    SET_MIN= 'SET_MIN',
    GET_MAX= 'GET_MAX',
    SET_MAX= 'SET_MAX',
    GET_OFFSET= 'GET_OFFSET',
    SET_OFFSET= 'SET_OFFSET',
    GET_STATE= 'GET_STATE',
    SET_STATE= 'SET_STATE',
    GET_REVERSE= 'GET_REVERSE',
    SET_REVERSE= 'SET_REVERSE',
    GET_STATE_PCA= 'GET_STATE_PCA',
    SET_STATE_PCA= 'SET_STATE_PCA',
}

export const CommandServoId = '03';

export default class ClusterServo extends Cluster {
    constructor() {
        super(
            ClusterName.SERVO,
            CommandServoId,
            [
                { code: '00', name: ClusterServoCommands.GET_ALL},
                { code: '01', name: ClusterServoCommands.GET_ANGLE},
                { code: '02', name: ClusterServoCommands.SET_ANGLE},
                { code: '03', name: ClusterServoCommands.GET_MIN},
                { code: '04', name: ClusterServoCommands.SET_MIN},
                { code: '05', name: ClusterServoCommands.GET_MAX},
                { code: '06', name: ClusterServoCommands.SET_MAX},
                { code: '07', name: ClusterServoCommands.GET_OFFSET},
                { code: '08', name: ClusterServoCommands.SET_OFFSET},
                { code: '09', name: ClusterServoCommands.GET_STATE},
                { code: '0A', name: ClusterServoCommands.SET_STATE},
                { code: '0B', name: ClusterServoCommands.GET_REVERSE},
                { code: '0C', name: ClusterServoCommands.SET_REVERSE},
                { code: '0D', name: ClusterServoCommands.GET_STATE_PCA},
                { code: '0E', name: ClusterServoCommands.SET_STATE_PCA},
                { code: 'FF', name: ClusterGenericCommands.GENERIC }
            ]
        );
    }
}
