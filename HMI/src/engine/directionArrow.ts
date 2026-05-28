import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import { ClusterName } from "../communication/clusters/clusterType.ts";
import { ClusterServoCommands } from "../communication/clusters/clusterServo.ts";
import { ClusterBodyCommands } from "../communication/clusters/clusterBody.ts";
import { CircleGeometry, DoubleSide, MathUtils, Mesh, MeshBasicMaterial, Object3D } from "three";
import { HexapodStruct } from "../entity/hexapod.ts";

export default class DirectionArrow extends Object3D {
    socket: Socket;
    right: HTMLElement;
    forward: HTMLElement;
    left: HTMLElement;
    backward: HTMLElement;
    stop: HTMLElement;
    frontLeft: HTMLElement | null;
    frontRight: HTMLElement | null;
    backLeft: HTMLElement | null;
    backRight: HTMLElement | null;
    enable: HTMLElement;
    speedControl: HTMLInputElement | null;
    speedValue: HTMLElement | null;
    rotationControl: HTMLInputElement | null;
    rotationValue: HTMLElement | null;
    gaitSelect: HTMLSelectElement | null;
    directionButtons: HTMLElement[];
    circle: Mesh;
    hexapodStruct: HexapodStruct;

    constructor(socket: Socket, hexapodStruct: HexapodStruct) {
        super();
        this.socket = socket;
        this.hexapodStruct = hexapodStruct;
        this.forward = document.getElementById('forward')!;
        this.right = document.getElementById('right')!;
        this.left = document.getElementById('left')!;
        this.backward = document.getElementById('backward')!;
        this.stop = document.getElementById('stop')!;
        this.frontLeft = document.getElementById('joy-nw');
        this.frontRight = document.getElementById('joy-ne');
        this.backLeft = document.getElementById('joy-sw');
        this.backRight = document.getElementById('joy-se');
        this.enable = document.getElementById('enable')!;
        this.speedControl = document.getElementById('cockpit-speed') as HTMLInputElement | null;
        this.speedValue = document.getElementById('cockpit-speed-value');
        this.rotationControl = document.getElementById('cockpit-rotation') as HTMLInputElement | null;
        this.rotationValue = document.getElementById('cockpit-rotation-value');
        this.gaitSelect = document.getElementById('cockpit-gait') as HTMLSelectElement | null;
        this.directionButtons = [
            this.frontLeft,
            this.forward,
            this.frontRight,
            this.left,
            this.stop,
            this.right,
            this.backLeft,
            this.backward,
            this.backRight,
        ].filter((button): button is HTMLElement => button !== null);

        this.initializeButtons();
        this.syncMotionControls();

        const geometry = new CircleGeometry(0.3, 32, -Math.PI / 2 - Math.PI / 16 / 2, Math.PI / 16);
        geometry.rotateX(Math.PI / 2);
        geometry.rotateY(Math.PI);
        geometry.translate(0, 1.11, -0.5);
        const material = new MeshBasicMaterial({ color: 0x4a6a8f });
        material.side = DoubleSide;
        this.circle = new Mesh(geometry, material);
        //this.circle.visible = false;
        this.add(this.circle);

        this.socket.addSpecificCallbackRead(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS, (message: Message) => {
            if (message.params && message.params.length === 11) {
                this.setDirection(message.getValueUint16(4));
                this.hexapodStruct.turningRate = message.getValueUint16(6);
                this.hexapodStruct.clockwise = message.getValueUint8(8) === 1;
                this.hexapodStruct.duration = message.getValueUint16(9);
                this.syncMotionControls();
            }
        });

    }

    private gaitToCode(gait: string): number {
        if (gait === 'WAVE') return 1;
        if (gait === 'RIPPLE') return 2;
        if (gait === 'DOUBLE_WAVE') return 3;
        return 0;
    }

    private setWalkStatus(walking: boolean) {
        this.socket.write(
            new Message(
                ClusterName.BODY,
                ClusterBodyCommands.SET_WALK_STATUS,
                [walking ? 0 : 2, this.hexapodStruct.duration],
                [0xFF, 0xFFFF],
            ),
        );
    }

    private updateDirectionSelection(selected: HTMLElement | null) {
        this.directionButtons.forEach((button) => button.classList.remove('select'));
        if (selected) {
            selected.classList.add('select');
        }
    }

    private sendDirection(angleDeg: number, selected: HTMLElement | null) {
        this.updateDirectionSelection(selected);
        this.hexapodStruct.turningRate = 0;
        this.socket.write(
            new Message(
                ClusterName.BODY,
                ClusterBodyCommands.SET_ROTATION,
                [0, this.hexapodStruct.clockwise ? 1 : 0],
                [0xFFFF, 0xFF],
            ),
        );
        this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_DIRECTION, [angleDeg], [0xFFFF]));
        this.setWalkStatus(true);
        this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS));
    }

    private speedPercentFromDuration(duration: number): number {
        const clampedDuration = Math.max(1000, Math.min(10000, duration));
        return Math.round(((10000 - clampedDuration) / 9000) * 100);
    }

    private durationFromSpeedPercent(speedPercent: number): number {
        const clampedSpeed = Math.max(0, Math.min(100, speedPercent));
        return Math.round(10000 - ((9000 * clampedSpeed) / 100));
    }

    private signedRotation(): number {
        const magnitude = this.hexapodStruct.turningRate;
        return this.hexapodStruct.clockwise ? -magnitude : magnitude;
    }

    private formatSignedRotation(signed: number): string {
        if (signed === 0) return '0';
        return signed > 0 ? `↺ ${signed}` : `↻ ${Math.abs(signed)}`;
    }

    private syncMotionControls() {
        if (this.speedControl) {
            const speed = this.speedPercentFromDuration(this.hexapodStruct.duration);
            this.speedControl.value = speed.toString();
            if (this.speedValue) {
                this.speedValue.innerText = `${speed}%`;
            }
        }

        if (this.rotationControl) {
            const signed = this.signedRotation();
            this.rotationControl.value = signed.toString();
            if (this.rotationValue) {
                this.rotationValue.innerText = this.formatSignedRotation(signed);
            }
        }

        if (this.gaitSelect) {
            this.gaitSelect.value = this.hexapodStruct.gait;
        }
    }

    initializeButtons() {
        this.forward.addEventListener('click', () => {
            this.sendDirection(0, this.forward);
        });
        this.frontRight?.addEventListener('click', () => {
            this.sendDirection(315, this.frontRight);
        });
        this.right.addEventListener('click', () => {
            this.sendDirection(270, this.right);
        });
        this.backRight?.addEventListener('click', () => {
            this.sendDirection(225, this.backRight);
        });
        this.left.addEventListener('click', () => {
            this.sendDirection(90, this.left);
        });
        this.frontLeft?.addEventListener('click', () => {
            this.sendDirection(45, this.frontLeft);
        });
        this.backward.addEventListener('click', () => {
            this.sendDirection(180, this.backward);
        });
        this.backLeft?.addEventListener('click', () => {
            this.sendDirection(135, this.backLeft);
        });
        this.stop.addEventListener('click', () => {
            this.updateDirectionSelection(this.stop);
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_DIRECTION, [0], [0xFFFF]));
            this.setWalkStatus(false);
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.GET_ALL_PARAMS));
        });

        this.speedControl?.addEventListener('input', () => {
            if (!this.speedControl || !this.speedValue) {
                return;
            }
            this.speedValue.innerText = `${this.speedControl.value}%`;
        });
        this.speedControl?.addEventListener('change', () => {
            if (!this.speedControl) {
                return;
            }
            const speed = Number(this.speedControl.value);
            const duration = this.durationFromSpeedPercent(speed);
            this.hexapodStruct.duration = duration;
            this.socket.write(new Message(ClusterName.BODY, ClusterBodyCommands.SET_DURATION, [duration], [0xFFFF]));
        });

        this.rotationControl?.addEventListener('input', () => {
            if (!this.rotationControl || !this.rotationValue) {
                return;
            }
            const signed = Number(this.rotationControl.value);
            this.rotationValue.innerText = this.formatSignedRotation(signed);
        });
        this.rotationControl?.addEventListener('change', () => {
            if (!this.rotationControl) {
                return;
            }
            const signed = Number(this.rotationControl.value);
            this.hexapodStruct.turningRate = Math.abs(signed);
            this.hexapodStruct.clockwise = signed < 0;
            this.socket.write(
                new Message(
                    ClusterName.BODY,
                    ClusterBodyCommands.SET_ROTATION,
                    [this.hexapodStruct.turningRate, this.hexapodStruct.clockwise ? 1 : 0],
                    [0xFFFF, 0xFF],
                ),
            );
        });

        this.gaitSelect?.addEventListener('change', () => {
            if (!this.gaitSelect) {
                return;
            }
            this.hexapodStruct.gait = this.gaitSelect.value as HexapodStruct['gait'];
            this.socket.write(
                new Message(ClusterName.BODY, ClusterBodyCommands.SET_GAIT, [this.gaitToCode(this.hexapodStruct.gait)]),
            );
        });

        this.enable.addEventListener('click', () => {
            this.enable.classList.toggle('select');
            if (this.enable.classList.contains('select')) {
                this.enableServos();
            } else {
                this.disableServos();
            }
        });
        this.socket.addCallbackStopped(() => {
            this.enable.classList.remove('select');
            this.disableServos();
        });
    }

    setDirection(direction: number) {
        // Geometry pre-rotated by π to start at -Z (forward). degToRad rotates CCW matching convention.
        // Convention: 0°=forward, 90°=left, 180°=back, 270°=right.
        this.circle.rotation.y = (MathUtils.degToRad(direction));
    }

    enableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE, [i, 1]));
        }
        this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [1]));
        this.enable.children[0].classList.remove('bi-toggle2-off');
        this.enable.children[0].classList.add('bi-toggle2-on');
        this.enable.children[0].classList.add('select');
        this.circle.visible = true;
    }

    disableServos() {
        for (let i = 0; i < 18; i++) {
            this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE, [i, 0]));
        }
        this.socket.write(new Message(ClusterName.SERVO, ClusterServoCommands.SET_STATE_PCA, [0]));
        this.enable.children[0].classList.remove('bi-toggle2-on');
        this.enable.children[0].classList.add('bi-toggle2-off');
        this.enable.children[0].classList.remove('select');
        this.circle.visible = false;
    }
}