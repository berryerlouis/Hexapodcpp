import Ui from './ui.ts'
import Socket from "./communication/socket.ts";
import Cockpit from "./engine/cockpit.ts";

const socket:Socket = new Socket();
new Ui(socket);
new Cockpit(socket);

