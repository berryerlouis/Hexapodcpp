import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterBodyCommands {
    GET_ALL_PARAMS = 'GET_ALL_PARAMS',
    SET_BODY_X_Y_Z = 'SET_BODY_X_Y_Z',
    SET_LEG_X_Y_Z = 'SET_LEG_X_Y_Z',
    SET_WALK_STATUS = 'SET_WALK_STATUS',
    SET_DIRECTION = 'SET_DIRECTION',
    GET_DIRECTION = 'GET_DIRECTION',
    SET_AMPLITUDE = 'SET_AMPLITUDE',
    GET_AMPLITUDE = 'GET_AMPLITUDE',
    SET_ELEVATION = 'SET_ELEVATION',
    GET_ELEVATION = 'GET_ELEVATION',
    GET_DURATION = 'GET_DURATION',
    SET_DURATION = 'SET_DURATION',
    GET_GAIT = 'GET_GAIT',
    SET_GAIT = 'SET_GAIT',
    SET_ROTATION = 'SET_ROTATION',
    GET_ROTATION = 'GET_ROTATION',
}

export const CommandBodyId = '05';

export default class ClusterBody extends Cluster {
    constructor() {
        super(
            ClusterName.BODY,
            CommandBodyId,
            [
                {code: '00', name: ClusterBodyCommands.GET_ALL_PARAMS},
                {code: '01', name: ClusterBodyCommands.SET_BODY_X_Y_Z},
                {code: '02', name: ClusterBodyCommands.SET_LEG_X_Y_Z},
                {code: '03', name: ClusterBodyCommands.SET_WALK_STATUS},
                {code: '04', name: ClusterBodyCommands.SET_DIRECTION},
                {code: '05', name: ClusterBodyCommands.GET_DIRECTION},
                {code: '06', name: ClusterBodyCommands.SET_AMPLITUDE},
                {code: '07', name: ClusterBodyCommands.GET_AMPLITUDE},
                {code: '08', name: ClusterBodyCommands.SET_ELEVATION},
                {code: '09', name: ClusterBodyCommands.GET_ELEVATION},
                {code: '0A', name: ClusterBodyCommands.GET_DURATION},
                {code: '0B', name: ClusterBodyCommands.SET_DURATION},
                {code: '0C', name: ClusterBodyCommands.GET_GAIT},
                {code: '0D', name: ClusterBodyCommands.SET_GAIT},
                {code: '0E', name: ClusterBodyCommands.SET_ROTATION},
                {code: '0F', name: ClusterBodyCommands.GET_ROTATION},
                {code: 'FF', name: ClusterGenericCommands.GENERIC}
            ]
        );
    }
}
