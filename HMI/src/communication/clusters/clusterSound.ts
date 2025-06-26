import {ClusterName, Cluster, ClusterGenericCommands} from "./clusterType.ts";

export enum ClusterSoundCommands {
    SOUND_STATUS = 'SOUND_STATUS'
}

export const CommandSoundId = '07';

export default class ClusterSound extends Cluster {
    constructor() {
        super(
            ClusterName.SOUND,
            CommandSoundId,
            [
                {code: '00', name: ClusterSoundCommands.SOUND_STATUS},
                {code: 'FF', name: ClusterGenericCommands.GENERIC}
            ]
        );
    }
}
