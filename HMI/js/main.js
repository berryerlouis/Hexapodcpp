import { SerialInterface } from './protocol/Serial.js'
import { MessageManager } from './protocol/MessageManager.js'
import { ClusterName, CommandBattery, CommandGeneral } from './protocol/Cluster.js'
import { Message } from './protocol/Message.js'
import { Logger } from './ui/logger/Logger.js'
import { Treeview } from './ui/treeview/Treeview.js'
import { Command } from './ui/Command.js'

const serialInterface = new SerialInterface();
const messageManager = new MessageManager(serialInterface);
const logger = new Logger(messageManager);
const treeview = new Treeview(messageManager);
const menu = new Command();

messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.GENERAL, CommandGeneral.VERSION, (message) => {
    if (message.size > 0) {
        let major = message.fetchInt8U();
        let minor = message.fetchInt8U();
        $('#hexapod-version').text(major + '.' + minor);
    }
});

messageManager.addCallbackNotifyOnSpecificCommand(ClusterName.BATTERY, CommandBattery.STATUS, (message) => {
    if (message.size > 0) {
        let state = message.fetchInt8U();
        let voltage = message.fetchInt16U() / 100;
        if (state == 0) {
            $('#hexapod-battery-status').removeClass('bi-battery');
            $('#hexapod-battery-status').removeClass('bi-battery-half');
            $('#hexapod-battery-status').addClass('bi-battery-full');
            $('#hexapod-battery-status').attr('style', "color: rgb(50, 223, 27);");
        }
        else if (state == 1) {
            $('#hexapod-battery-status').removeClass('bi-battery');
            $('#hexapod-battery-status').removeClass('bi-battery-full');
            $('#hexapod-battery-status').addClass('bi-battery-half');
            $('#hexapod-battery-status').attr('style', "color: rgb(223, 135, 27);");
        }
        else if (state == 2) {
            $('#hexapod-battery-status').removeClass('bi-battery-full');
            $('#hexapod-battery-status').removeClass('bi-battery-half');
            $('#hexapod-battery-status').addClass('bi-battery');
            $('#hexapod-battery-status').attr('style', "color: rgb(223, 27, 27);");
        }
        else {
            $('#hexapod-battery-status').removeClass('bi-battery-full');
            $('#hexapod-battery-status').removeClass('bi-battery-half');
            $('#hexapod-battery-status').addClass('bi-battery');
            $('#hexapod-battery-status').attr('style', "color: rgb(223, 27, 27);");
        }
        $('#hexapod-battery-voltage').text(voltage.toFixed(2) + 'V');
    }
});



$('#connect-button').click(async () => {
    await serialInterface.init(navigator);



    messageManager.write(new Message().build("Tx", ClusterName.GENERAL, CommandGeneral.VERSION));

    messageManager.write(new Message().build("Tx", ClusterName.BATTERY, CommandBattery.STATUS));
});

function update() {

    if (serialInterface.messageAvailable()) {
        const frame = serialInterface.popMessage();
    }
    messageManager.update();
}

function animate() {
    update();
    requestAnimationFrame(animate);
}

animate();