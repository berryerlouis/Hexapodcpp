import Ui from './ui.ts'
import Socket from "./communication/socket.ts";
import Cockpit from "./engine/cockpit.ts";

//const socket:Socket = new Socket('ws://88.183.190.4:58080');
const socket: Socket = new Socket('ws://127.0.0.1:8080');
new Ui(socket);
new Cockpit(socket);

