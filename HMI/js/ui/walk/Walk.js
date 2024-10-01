import {Message} from "../../protocol/Message.js";
import {ClusterName, CommandServo} from "../../protocol/Cluster.js";

export default class Walk {

    constructor(messageManager) {
        this.messageManager = messageManager;
    }

    init() {
        for (let i = 0; i < 18; i++) {
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_ANGLE, 1, [i.toString(16)]));
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_MIN, 1, [i.toString(16)]));
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_MAX, 1, [i.toString(16)]));
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_OFFSET, 1, [i.toString(16)]));
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_REVERSE, 1, [i.toString(16)]));
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_STATE, 1, [i.toString(16)]));
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_STATE, 2, [i.toString(16), "01"]));
            this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_ANGLE, 2, [i.toString(16), (90).toString(16)]));
        }


        /*this.step = 0;
        setInterval(() => {
            switch (this.step) {
                case 0:
                    this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_ANGLE, 2, ["0C", (100).toString(16)]));
                    this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_ANGLE, 2, ["0D", (90).toString(16)]));
                    break;
                case 1:
                    this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_ANGLE, 2, ["0C", (80).toString(16)]));
                    this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_ANGLE, 2, ["0D", (90).toString(16)]));
                    break;
                case 2:
                    this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_ANGLE, 2, ["0C", (90).toString(16)]));
                    this.messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.SET_ANGLE, 2, ["0D", (100).toString(16)]));
                    this.step = -1;
                    break;
            }
            this.step++;
        }, 2000);*/
    }
}