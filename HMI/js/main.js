import {SerialInterface} from './protocol/Serial.js'
import {MessageManager} from './protocol/MessageManager.js'
import {Logger} from './ui/logger/Logger.js'
import {Treeview} from './ui/treeview/Treeview.js'
import {Command} from './ui/Command.js'
import Canvas from "./ui/Canvas.js";
import {DatabaseManager} from "./clusters/DatabaseManager.js";
import Hexapod from "./ui/hexapod/hexapod.js";
import Controls from "./ui/gui/Controls.js";
import Walk from "./ui/walk/Walk.js";
import {Message} from "./protocol/Message.js";
import {ClusterName, CommandBattery, CommandGeneral, CommandImu} from "./protocol/Cluster.js";


const serialInterface = new SerialInterface();
const messageManager = new MessageManager(serialInterface);
const logger = new Logger(messageManager);
const treeview = new Treeview(messageManager);
const menu = new Command();
const canvas = new Canvas(
    window.innerWidth,
    window.innerHeight - document.body.children[0].children[0].clientHeight - document.body.children[2].children[0].clientHeight
);
const robot = new Hexapod(canvas.groupBody);
const guiControls = new Controls(messageManager, robot);
const databaseManager = new DatabaseManager(messageManager, robot);
const walk = new Walk(messageManager);

$('#connect-button').click(async () => {
    await serialInterface.init(navigator);
    walk.init();
    messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));
    messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
    walk.init();
    let toto = 0;
    setInterval(() => {
        messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.ROULIS));
        messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.ACC));
        messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.GYR));
        //messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_ALL));
    }, 1000);
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
    guiControls.updateDisplay();
}


init();