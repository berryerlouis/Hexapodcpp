import Ui from './ui.ts'
import Socket from "./communication/socket.ts";
import {Direction} from "./communication/protocol.ts";
import {ClusterGeneralCommands} from "./communication/clusters/clusterGeneral.ts";
import {ClusterName} from "./communication/clusters/clusterType.ts";
import Message from "./communication/message.ts";
import {ClusterServoCommands} from "./communication/clusters/clusterServo.ts";


const socket:Socket = new Socket(()=>{
    init();

    socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.VERSION, (message:Message) => {
        document.getElementById('version')!.innerText = "V"+ message.params[0].toString() + "." + message.params[1].toString() ;
    });
});
new Ui(socket);


document.getElementById('connect-button')!.addEventListener('click',(() : void => {
    init();
}));

function init() {
    socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.GET_STATE_PCA));

    for (let i = 0; i < 18; i++) {
        socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.GET_ANGLE, 1, [i]));
        socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.GET_MIN, 1, [i]));
        socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.GET_MAX, 1, [i]));
        socket.write(new Message(Direction.TX, ClusterName.SERVO, ClusterServoCommands.GET_STATE, 1, [i]));
    }
    socket.write(new Message(Direction.TX, ClusterName.GENERAL, ClusterGeneralCommands.VERSION));

}