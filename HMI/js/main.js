import {SerialInterface} from './protocol/Serial.js'
import {MessageManager} from './protocol/MessageManager.js'
import {ClusterName, CommandBattery, CommandGeneral} from './protocol/Cluster.js'
import {Message} from './protocol/Message.js'
import {Logger} from './ui/logger/Logger.js'
import {Treeview} from './ui/treeview/Treeview.js'
import {Command} from './ui/Command.js'
import {ClusterBattery} from "./ui/clusters/battery/ClusterBattery.js";
import {ClusterGeneral} from "./ui/clusters/general/ClusterGeneral.js";
import Canvas from "./ui/Canvas.js";

const canvas = new Canvas(window.innerWidth, window.innerHeight - document.body.children[0].children[0].clientHeight);
const serialInterface = new SerialInterface();
const messageManager = new MessageManager(serialInterface);
const logger = new Logger(messageManager);
const treeview = new Treeview(messageManager);
const menu = new Command();
const cluBattery = new ClusterBattery(messageManager);
const cluGeneral = new ClusterGeneral(messageManager);
const clusters = [cluBattery, cluGeneral];

$('#connect-button').click(async () => {
    await serialInterface.init(navigator);
    let toto = 0;
    setInterval(() => {
        if (toto === 0) {
            messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));
            toto = 1;
        } else {
            messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
            toto = 0;
        }
    }, 50);
});

function init() {
    clusters.forEach((cluster) => {
        cluster.initialize();
    })
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