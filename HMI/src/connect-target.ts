
export enum Target {
    X64 = 'X64',
    HEXAPOD = 'HEXAPOD',
}   

type Callback = (target: Target) => void;


export default class ConnectTarget {


    callback: Callback ;
    connectTarget: HTMLElement;
    connectX64: HTMLElement;
    connectBot: HTMLElement;
    modalConnect: HTMLElement;

    constructor(callback: Callback) {
        this.callback = callback;
        this.modalConnect = document.getElementById('modal-connect')!;
        this.connectTarget = document.getElementById('connect-target')!;
        this.connectX64 = document.getElementById('connect-x64')!;
        this.connectBot = document.getElementById('connect-bot')!;

        this.connectX64.addEventListener("click", () => { this.clickConnectTarget(Target.X64)});
        this.connectBot.addEventListener("click", () => { this.clickConnectTarget(Target.HEXAPOD)});
    }

    clickConnectTarget(target: Target) {
        let span = document.createElement("span");
        span.innerText = target;
        this.connectTarget.appendChild(span);
        this.modalConnect.style.display = "none";
        this.callback(target);
    }
}