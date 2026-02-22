import Ui from './ui.ts'
import Socket from "./communication/socket.ts";
import Cockpit from "./engine/cockpit.ts";
import ConnectTarget, { Target } from './connect-target.ts';

export enum IPTarget {
    X64 = '127.0.0.1:8080',
    HEXAPOD = '88.183.190.4:58080',
}   

new ConnectTarget((target: Target) => {
    let socket: Socket;
    if(target == Target.X64) {
        socket = new Socket(`ws://${IPTarget.X64}`);
    } else if (target == Target.HEXAPOD) {
        socket = new Socket(`ws://${IPTarget.HEXAPOD}`);
    } else {
        throw new Error('Invalid target');
    }
    new Ui(socket);
    new Cockpit(socket);
});

