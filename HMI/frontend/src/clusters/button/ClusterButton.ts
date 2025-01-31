import { ClusterName, CommandButton } from "../../protocol/Cluster.js";

export class ClusterButton {
    private messageManager: any;
    private databaseManager: any;
    private robot: any;

    constructor(messageManager: any, databaseManager: any, robot: any) {
        this.messageManager = messageManager;
        this.databaseManager = databaseManager;
        this.robot = robot;
        this.initialize();
    }

    initialize() {
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BUTTON, CommandButton.BP_STATUS, (message: any) => {
            if (message.size === 1) {
                // Add your logic here
            }
        });
    }
}