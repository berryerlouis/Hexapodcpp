import { ClusterName, CommandSound} from "../../protocol/Cluster.js";

export class ClusterSound {
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
        this.messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.SOUND, CommandSound.SOUND_STATUS, (message: any) => {
            if (message.size === 2) {
                // Add your logic here
            }
        });
    }
}