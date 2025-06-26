import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterButtonCommands {
    BP_STATUS = 'BP_STATUS'
}

export const CommandButtonId = '06';

export default class ClusterButton extends Cluster {
    constructor() {
        super(
            ClusterName.BUTTON,
            CommandButtonId,
            [
                {code: '00', name: ClusterButtonCommands.BP_STATUS},
                {code: 'FF', name: ClusterGenericCommands.GENERIC}
            ]
        );
    }
}
