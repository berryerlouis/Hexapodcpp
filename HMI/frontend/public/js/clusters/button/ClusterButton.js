import { ClusterName, CommandButton } from "../../protocol/Cluster.js";
export class ClusterButton {
    constructor(messageManager, databaseManager, robot) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize();
    }
    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BUTTON, CommandButton.BP_STATUS, (message) => {
            if (message.size === 1) {
                // Add your logic here
            }
        });
    }
}
