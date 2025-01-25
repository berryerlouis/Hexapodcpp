import { SerialInterface } from './protocol/Serial.js';
import { SocketInterface } from './protocol/Socket.js';
import { MessageManager } from './protocol/MessageManager.js';
import { Logger } from './ui/logger/Logger.js';
import { Treeview } from './ui/treeview/Treeview.js';
import { Command } from './ui/Command.js';
import Canvas from "./ui/Canvas.js";
import { DatabaseManager } from "./clusters/DatabaseManager.js";
import Hexapod from "./ui/hexapod/hexapod.js";
import Controls from "./ui/gui/Controls.js";
import Walk from "./ui/walk/Walk.js";
import Compass from "./ui/Compass.js";
import { ClusterName, CommandBattery, CommandGeneral, CommandImu, CommandServo } from "./protocol/Cluster.js";
import { Message } from "./protocol/Message.js";

const socketInterface = new SocketInterface();
const serialInterface = new SerialInterface();
const messageManager = new MessageManager(socketInterface);
const logger = new Logger(messageManager);
const treeview = new Treeview(messageManager);
const menu = new Command();
const compass = new Compass();
const canvas = new Canvas(
    window.innerWidth,
    window.innerHeight - document.body.children[0].children[0].clientHeight - document.body.children[2].children[0].clientHeight
);
const robot = new Hexapod(canvas.groupBody);
const guiControls = new Controls(messageManager, robot);
const databaseManager = new DatabaseManager(messageManager, robot, compass);
const walk = new Walk(messageManager);

$('#connect-button').click(async () => {
    //await serialInterface.init(navigator);
    ///messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
    messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.PRESSURE));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.ALTITUDE));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.TMPBAR));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.TMP));
    messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_ALL));
    walk.init();

    setTimeout(() => {
        setInterval(() => {
            messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_ALL));
            //messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));
        }, 1000);

        setInterval(() => {
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.ALL));
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.YAWPITCHROLL));
        }, 1000);

        setInterval(() => {
            messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.PRESSURE));
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.ALTITUDE));
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.TMPBAR));
        }, 10000);
    }, 1000);
});

function init() {
    animate();
}

function update() {
    messageManager.update();
    canvas.animate();
    compass.animate();
}

function animate() {
    update();
    requestAnimationFrame(animate);
    guiControls.updateDisplay();
}

init();