import {ClusterName, CommandProximity} from "../../protocol/Cluster.js";

export class ClusterProximity {
    constructor(messageManager, databaseManager) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.LASER, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'PROXIMITY',
                    command: 'LASER',
                    value: message.fetchInt16U()
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.US_LEFT, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'PROXIMITY',
                    command: 'US_LEFT',
                    value: message.fetchInt16U()
                });
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.US_RIGHT, (message) => {
            if (message.size > 0) {
                this.databaseManager.updateDb({
                    cluster: 'PROXIMITY',
                    command: 'US_RIGHT',
                    value: message.fetchInt16U()
                });
            }
        });
    }
}

