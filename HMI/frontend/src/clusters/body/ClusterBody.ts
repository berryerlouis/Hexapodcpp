import { ClusterName, CommandBody } from "../../protocol/Cluster.js";

export class ClusterBody {
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
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BODY, CommandBody.SET_BODY_X_Y_Z, (message: any) => {
            if (message.size === 4) {
                // Add your logic here
            }
        });
    }
}