import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import {ClusterName} from "../communication/clusters/clusterType.ts";
import { ClusterBodyCommands } from "../communication/clusters/clusterBody.ts";

// Walk status values exchanged with the bot (see EWalkStatus in C++):
//   0 = WALKING, 1 = transition, 2 = STOPPED.
const WALK_STATUS_STOPPED = 2;

export default class Cockpit {
    cockpitEnable: HTMLElement;
    cockpitDirection: HTMLElement;
    cockpitEnableIcon: HTMLElement | null;
    socket: Socket;

    constructor(socket: Socket) {
        this.socket = socket;
        this.cockpitEnable = document.getElementById('enable')!;
        this.cockpitDirection = document.getElementById('direction')!;
        this.cockpitEnableIcon = this.cockpitEnable.querySelector('i');
        this.cockpitEnable.addEventListener('click', () => {
            this.cockpitEnable.classList.contains('select') ? this.enableCockpitDirection() : this.disableCockpitDirection();
        });

        this.socket.addCallbackStarted(() => {
            this.enableCockpit();
        });
        this.socket.addCallbackStopped(() => {
            this.disableCockpit();
        });

        // Mirror the bot's walk status: show the direction joystick while walking,
        // hide it on emergency / regular stop.
        this.socket.addSpecificCallbackRead(
            ClusterName.BODY,
            ClusterBodyCommands.GET_ALL_PARAMS,
            (message: Message) => {
                if (!message.params || message.params.length !== 11) {
                    return;
                }
                const status = message.getValueUint8(0);
                if (status === WALK_STATUS_STOPPED) {
                    this.disableCockpitDirection();
                } else {
                    this.enableCockpitDirection();
                }
            },
        );
    }

    private disableCockpit() {
        this.cockpitEnable.setAttribute('style', 'display:none');
        this.disableCockpitDirection();
    }

    private enableCockpit() {
        this.cockpitEnable.setAttribute('style', 'display:flex');
    }

    private disableCockpitDirection() {
        this.cockpitDirection.setAttribute('style', 'display:none');
        this.cockpitEnable.classList.remove('select');
        this.setEnableIcon(false);
    }

    private enableCockpitDirection() {
        this.cockpitDirection.setAttribute('style', 'display:flex');
        this.cockpitEnable.classList.add('select');
        this.setEnableIcon(true);
    }

    private setEnableIcon(on: boolean) {
        if (!this.cockpitEnableIcon) {
            return;
        }
        this.cockpitEnableIcon.classList.remove('bi-toggle2-on', 'bi-toggle2-off');
        this.cockpitEnableIcon.classList.add(on ? 'bi-toggle2-on' : 'bi-toggle2-off');
    }
}