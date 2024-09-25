import {ClusterBattery} from "./battery/ClusterBattery.js";
import {ClusterGeneral} from "./general/ClusterGeneral.js";
import {ClusterProximity} from "./proximity/ClusterProximity.js";
import {ClusterImu} from "./imu/ClusterImu.js";
import {ClusterServo} from "./servo/ClusterServo.js";
import {clustersDatabase} from "./db.js"

export class DatabaseManager {
    constructor(messageManager) {
        this.messageManager = messageManager;
        this.cluBattery = new ClusterBattery(messageManager, this);
        this.cluGeneral = new ClusterGeneral(messageManager, this);
        this.cluProximity = new ClusterProximity(messageManager, this);
        this.clusterImu = new ClusterImu(messageManager, this);
        this.clusterServo = new ClusterServo(messageManager, this);
        this.clusters = [
            this.cluBattery,
            this.cluGeneral,
            this.cluProximity,
            this.clusterImu,
            this.clusterServo,
        ];

        this.clusters.forEach((cluster) => {
            cluster.initialize();
        });
    }

    updateDb = ({cluster, command, item, value}) => {
        if (item) {
            clustersDatabase[cluster][command][item] = value;
            return;
        }
        clustersDatabase[cluster][command] = value;
    };
}