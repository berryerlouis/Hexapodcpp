import Ui from './ui.ts'
import Socket from "./communication/socket.ts";
import {ClusterGeneralCommands} from "./communication/clusters/clusterGeneral.ts";
import {ClusterName} from "./communication/clusters/clusterType.ts";
import Message from "./communication/message.ts";
import {ClusterServoCommands} from "./communication/clusters/clusterServo.ts";


const socket:Socket = new Socket();
new Ui(socket);

socket.addCallbackStarted(()=>{
    init();
    socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.VERSION, (message:Message) => {
        document.getElementById('version')!.innerText = "V"+ message.getValueUint8(0).toString() + "." + message.getValueUint8(1).toString();
    });
});

function init() {
    socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_STATE_PCA));

    for (let i = 0; i < 18; i++) {
        socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_ANGLE, 1, [i]));
        socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_MIN, 1, [i]));
        socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_MAX, 1, [i]));
        socket.write(new Message( ClusterName.SERVO, ClusterServoCommands.GET_STATE, 1, [i]));
    }
    socket.write(new Message( ClusterName.GENERAL, ClusterGeneralCommands.VERSION));

}