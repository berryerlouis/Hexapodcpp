import {ClusterName, CommandProximity} from "../../../protocol/Cluster.js";
import {clusters} from "../../../clusters/db.js";

export class ClusterProximity {
    constructor(messageManager) {
        this.messageManager = messageManager;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.LASER, (message) => {
            if (message.size > 0) {
                clusters['PROXIMITY'].LASER = message.fetchInt16U();
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.US_LEFT, (message) => {
            if (message.size > 0) {
                clusters['PROXIMITY'].US_LEFT = message.fetchInt16U();
            }
        });
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.PROXIMITY, CommandProximity.US_RIGHT, (message) => {
            if (message.size > 0) {
                clusters['PROXIMITY'].US_RIGHT = message.fetchInt16U();
            }
        });
    }
}

