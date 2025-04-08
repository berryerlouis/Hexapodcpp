import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterGeneralCommands {
    RESET= 'RESET',
    VERSION= 'VERSION',
    MIN_EXECUTION_TIME= 'MIN_EXECUTION_TIME',
    MAX_EXECUTION_TIME= 'MAX_EXECUTION_TIME',
    RESET_TIME= 'RESET_TIME'
}

export const CommandGeneralId = '00';

export default class ClusterGeneral extends Cluster {
    constructor() {
        super(
            ClusterName.GENERAL,
            CommandGeneralId,
            [
                { code: '00', name: ClusterGeneralCommands.RESET},
                { code: '01', name: ClusterGeneralCommands.VERSION },
                { code: '02', name: ClusterGeneralCommands.MIN_EXECUTION_TIME },
                { code: '03', name: ClusterGeneralCommands.MAX_EXECUTION_TIME },
                { code: '04', name: ClusterGeneralCommands.RESET_TIME },
                { code: 'FF', name: ClusterGenericCommands.GENERIC }
            ]
        );
    }
}
