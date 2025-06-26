import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterProximityCommands {
    US_LEFT = 'US_LEFT',
    US_RIGHT = 'US_RIGHT',
    LASER = 'LASER',
    SET_THRESHOLD = 'SET_THRESHOLD'
}

export const CommandProximityId = '02';

export default class ClusterProximity extends Cluster {
    constructor() {
        super(
            ClusterName.PROXIMITY,
            CommandProximityId,
            [
                {code: '00', name: ClusterProximityCommands.US_LEFT},
                {code: '01', name: ClusterProximityCommands.US_RIGHT},
                {code: '02', name: ClusterProximityCommands.LASER},
                {code: '03', name: ClusterProximityCommands.SET_THRESHOLD},
                {code: 'FF', name: ClusterGenericCommands.GENERIC}
            ]
        );
    }
}
