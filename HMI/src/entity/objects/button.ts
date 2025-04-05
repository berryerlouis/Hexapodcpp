import Message from "../../communication/message.ts";
import {ClusterName} from "../../communication/clusters/clusterType.ts";
import Socket from "../../communication/socket.ts";
import {ClusterButtonCommands} from "../../communication/clusters/clusterButton.ts";

interface ButtonStruct {
    pushed: boolean;
}

export default class Button {
    socket:Socket;
    buttonData: ButtonStruct = {pushed: false};
    constructor(socket: Socket) {
        this.socket = socket;

        socket.addSpecificCallbackRead(ClusterName.BUTTON, ClusterButtonCommands.BP_STATUS, (message:Message) => {
            this.buttonData.pushed = message.params[0] == 1;
            document.getElementById('button')!.innerText = this.buttonData.pushed ? 'Pushed' : 'Released';
        });
    }
}