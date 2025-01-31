var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
import { SocketInterface } from "./protocol/Socket.js";
import { MessageManager } from './protocol/MessageManager.js';
import Canvas from "./ui/Canvas.js";
import Compass from "./ui/Compass.js";
import Hexapod from "./ui/hexapod/hexapod.js";
import { Message } from "./protocol/Message.js";
import { Pane } from 'tweakpane';
import { ClusterName, CommandGeneral, CommandImu, CommandServo } from "./protocol/Cluster.js";
const socketInterface = new SocketInterface();
const messageManager = new MessageManager(socketInterface);
const canvas = new Canvas(window.innerWidth, window.innerHeight - Number($('#header').height()) - Number($('#footer').height()));
const hexapod = new Hexapod(canvas.groupBody);
const compass = new Compass();
function init() {
    animate();
    const pane = new Pane();
}
function update() {
    canvas.animate();
    compass.animate();
    messageManager.update();
}
function animate() {
    requestAnimationFrame(animate);
    update();
}
init();
$('#connect-button').click(() => __awaiter(void 0, void 0, void 0, function* () {
    //await serialInterface.init(navigator);
    ///messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
    messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.PRESSURE));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.ALTITUDE));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.TMPBAR));
    messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.TMP));
    messageManager.write(new Message().build("Tx", ClusterName.SERVO, CommandServo.GET_ALL));
    //walk.init();
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
            //messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.PRESSURE));
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.ALTITUDE));
            messageManager.write(new Message().build("Tx", ClusterName.IMU, CommandImu.TMPBAR));
        }, 10000);
    }, 1000);
}));
