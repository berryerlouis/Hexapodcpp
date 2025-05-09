import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterBodyCommands {
    SET_BODY_X_Y_Z = 'SET_BODY_X_Y_Z',
    SET_LEG_X_Y_Z = 'SET_LEG_X_Y_Z',
    SET_WALK_STATUS = 'SET_WALK_STATUS',
    SET_DIRECTION = 'SET_DIRECTION',
    SET_AMPLITUDE_ELEVATION = 'SET_AMPLITUDE_ELEVATION',
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
                { code: '04', name: ClusterBodyCommands.SET_AMPLITUDE_ELEVATION },
                { code: '05', name: ClusterBodyCommands.GET_DIRECTION_AMPLITUDE_ELEVATION },
                { code: 'FF', name: ClusterGenericCommands.GENERIC }
            ]
        );
    }
}
