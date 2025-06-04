import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterBodyCommands {
    SET_BODY_X_Y_Z = 'SET_BODY_X_Y_Z',
    SET_LEG_X_Y_Z = 'SET_LEG_X_Y_Z',
    SET_WALK_STATUS = 'SET_WALK_STATUS',
    SET_DIRECTION = 'SET_DIRECTION',
    GET_DIRECTION = 'GET_DIRECTION',
    SET_AMPLITUDE = 'SET_AMPLITUDE',
    GET_AMPLITUDE = 'GET_AMPLITUDE',
    SET_ELEVATION = 'SET_ELEVATION',
    GET_ELEVATION = 'GET_ELEVATION',
    GET_DIRECTION_AMPLITUDE_ELEVATION ='GET_DIRECTION_AMPLITUDE_ELEVATION'
}

export const CommandBodyId = '05';

export default class ClusterBody extends Cluster {
    constructor() {
        super(
            ClusterName.BODY,
            CommandBodyId,
            [
                { code: '00', name: ClusterBodyCommands.SET_BODY_X_Y_Z},
                { code: '01', name: ClusterBodyCommands.SET_LEG_X_Y_Z },
                { code: '02', name: ClusterBodyCommands.SET_WALK_STATUS },
                { code: '03', name: ClusterBodyCommands.SET_DIRECTION },
                { code: '04', name: ClusterBodyCommands.GET_DIRECTION },
                { code: '05', name: ClusterBodyCommands.SET_AMPLITUDE },
                { code: '06', name: ClusterBodyCommands.GET_AMPLITUDE },
                { code: '07', name: ClusterBodyCommands.SET_ELEVATION },
                { code: '08', name: ClusterBodyCommands.GET_ELEVATION },
                { code: '09', name: ClusterBodyCommands.GET_DIRECTION_AMPLITUDE_ELEVATION },
                { code: 'FF', name: ClusterGenericCommands.GENERIC }
            ]
        );
    }
}
