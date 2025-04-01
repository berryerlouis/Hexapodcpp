import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterBodyCommands {
    SET_BODY_X_Y_Z= 'SET_BODY_X_Y_Z',
    SET_LEG_X_Y_Z= 'SET_LEG_X_Y_Z',
    SET_WALK_STATUS= 'SET_WALK_STATUS'
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
                { code: 'FF', name: ClusterGenericCommands.GENERIC }
            ]
        );
    }
}
