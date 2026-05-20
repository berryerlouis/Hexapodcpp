import Socket from "../communication/socket.ts";

export default class Cockpit {
    cockpitEnable: HTMLElement;
    cockpitDirection: HTMLElement;
    socket: Socket;

    constructor(socket: Socket) {
        this.socket = socket;
        this.cockpitEnable = document.getElementById('enable')!;
        this.cockpitDirection = document.getElementById('direction')!;
        this.cockpitEnable.addEventListener('click', () => {
            this.cockpitEnable.classList.contains('select') ? this.enableCockpitDirection() : this.disableCockpitDirection();
        });

        this.socket.addCallbackStarted(() => {
            this.enableCockpit();
        });
        this.socket.addCallbackStopped(() => {
            this.disableCockpit();
        });
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
    }

    private enableCockpitDirection() {
        this.cockpitDirection.setAttribute('style', 'display:flex');
    }
}