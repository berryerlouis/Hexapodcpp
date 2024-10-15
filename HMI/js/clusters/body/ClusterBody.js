import {ClusterName, CommandBody} from "../../protocol/Cluster.js";

export class ClusterBody {
    constructor(messageManager, databaseManager, robot) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize()
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BODY, CommandBody.SET_BODY_X_Y_Z, (message) => {
            if (message.size === 4) {
            }
        });
    }
}

