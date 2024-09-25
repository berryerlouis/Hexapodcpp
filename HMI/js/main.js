import {SerialInterface} from './protocol/Serial.js'
import {MessageManager} from './protocol/MessageManager.js'
import {ClusterName, CommandBattery, CommandGeneral, CommandServo} from './protocol/Cluster.js'
import {Message} from './protocol/Message.js'
import {Logger} from './ui/logger/Logger.js'
import {Treeview} from './ui/treeview/Treeview.js'
import {Command} from './ui/Command.js'
import Canvas from "./ui/Canvas.js";
import {DatabaseManager} from "./clusters/DatabaseManager.js";


const serialInterface = new SerialInterface();
const messageManager = new MessageManager(serialInterface);
const logger = new Logger(messageManager);
const treeview = new Treeview(messageManager);
const databaseManager = new DatabaseManager(messageManager);
const menu = new Command();
const canvas = new Canvas(
    window.innerWidth,
    window.innerHeight - document.body.children[0].children[0].clientHeight - document.body.children[2].children[0].clientHeight,
    messageManager
);

$('#connect-button').click(async () => {
    await serialInterface.init(navigator);


    for (let i = 0; i < 18; i++) {
        messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_ANGLE, 1, [i.toString(16)]));
        messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_MIN, 1, [i.toString(16)]));
        messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_MAX, 1, [i.toString(16)]));
        messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_OFFSET, 1, [i.toString(16)]));
        messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_REVERSE, 1, [i.toString(16)]));
        messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_STATE, 1, [i.toString(16)]));
    }

    let toto = 0;
    setInterval(() => {
        if (toto === 0) {
            messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));
            toto++;
        } else if (toto === 1) {
            messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
            toto = 0;
        } else if (toto === 2) {

            toto = 0;
        }
    }, 5000);
});

function init() {
    animate();
}

function update() {
    messageManager.update();
    canvas.animate();
}

function animate() {
    update();
    requestAnimationFrame(animate);
}


init();