import Hexapod from "../entity/hexapod.ts";
import Socket from "../communication/socket.ts";
import Message from "../communication/message.ts";
import { ClusterGenericCommands, ClusterName } from "../communication/clusters/clusterType.ts";
import { ClusterSoundCommands } from "../communication/clusters/clusterSound.ts";
import { ClusterProximityCommands } from "../communication/clusters/clusterProximity.ts";
import { ClusterServoCommands } from "../communication/clusters/clusterServo.ts";
import { ClusterGeneralCommands } from "../communication/clusters/clusterGeneral.ts";
import { ClusterImuCommands } from "../communication/clusters/clusterImu.ts";
import { ClusterBodyCommands } from "../communication/clusters/clusterBody.ts";

type GaitOption = "TRIPOD" | "WAVE" | "RIPPLE" | "DOUBLE_WAVE";

interface NumericControl {
    range: HTMLInputElement;
    number: HTMLInputElement;
    value: HTMLElement;
    setValue: (next: number) => void;
    setDisabled: (disabled: boolean) => void;
}

interface ServoControl {
    status: HTMLInputElement;
    min: NumericControl;
    max: NumericControl;
    angle: NumericControl;
    angleLive: HTMLElement;
}

interface LegVectorControl {
    x: NumericControl;
    y: NumericControl;
    z: NumericControl;
}

interface TraceSample {
    t: number;
    v: number;
}

interface ReadOnlyTrace {
    canvas: HTMLCanvasElement;
    samples: TraceSample[];
    lastSampleAt: number;
    minValue: number;
    maxValue: number;
}

const BODY_DURATION_MS = 1000;
const TRACE_WINDOW_MS = 100000;
const TRACE_SAMPLE_INTERVAL_MS = 50;
const MIN_DURATION_MS = 1000;
const MAX_DURATION_MS = 10000;

export default class Panel {
    private root: HTMLElement;
    private hexapod: Hexapod;
    private socket: Socket;
    private initDone: boolean;

    private bodyPosition: LegVectorControl | null;
    private bodyRotation: LegVectorControl | null;
    private legVectors: LegVectorControl[];

    private durationControl: NumericControl | null;
    private directionControl: NumericControl | null;
    private gaitSelect: HTMLSelectElement | null;
    private rotationControl: NumericControl | null;
    private amplitudeControl: NumericControl | null;
    private elevationControl: NumericControl | null;

    private servoMasterToggle: HTMLInputElement | null;
    private servoControls: Map<number, ServoControl>;
    private servoLegCards: HTMLElement[];

    private proximityLeft: HTMLElement | null;
    private proximityFront: HTMLElement | null;
    private proximityRight: HTMLElement | null;
    private soundLeft: HTMLElement | null;
    private soundRight: HTMLElement | null;

    private batteryVoltage: HTMLElement | null;
    private batteryCurrent: HTMLElement | null;
    private imuTemp: HTMLElement | null;
    private imuYaw: HTMLElement | null;
    private imuPitch: HTMLElement | null;
    private imuRoll: HTMLElement | null;
    private imuAcc: HTMLElement | null;
    private imuGyro: HTMLElement | null;
    private imuMag: HTMLElement | null;
    private readOnlyTraces: Map<HTMLElement, ReadOnlyTrace>;

    constructor(domElement: HTMLElement | undefined, hexapod: Hexapod, socket: Socket) {
        if (!domElement) {
            throw new Error("Panel container is missing");
        }

        this.root = domElement;
        this.hexapod = hexapod;
        this.socket = socket;
        this.initDone = false;

        this.bodyPosition = null;
        this.bodyRotation = null;
        this.legVectors = [];

        this.durationControl = null;
        this.directionControl = null;
        this.gaitSelect = null;
        this.rotationControl = null;
        this.amplitudeControl = null;
        this.elevationControl = null;

        this.servoMasterToggle = null;
        this.servoControls = new Map<number, ServoControl>();
        this.servoLegCards = [];

        this.proximityLeft = null;
        this.proximityFront = null;
        this.proximityRight = null;
        this.soundLeft = null;
        this.soundRight = null;

        this.batteryVoltage = null;
        this.batteryCurrent = null;
        this.imuTemp = null;
        this.imuYaw = null;
        this.imuPitch = null;
        this.imuRoll = null;
        this.imuAcc = null;
        this.imuGyro = null;
        this.imuMag = null;
        this.readOnlyTraces = new Map<HTMLElement, ReadOnlyTrace>();

        this.buildUI();
        this.registerSocketCallbacks();
    }

    update() {
        this.syncFromModel();
    }

    private registerSocketCallbacks() {
        this.socket.addSpecificCallbackRead(ClusterName.GENERAL, ClusterGeneralCommands.VERSION, () => {
            this.initDone = true;
        });

        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_STATE_PCA, (message: Message) => {
            if (!message.params) {
                return;
            }

            const enabled = message.getValueBool(0);
            if (this.servoMasterToggle) {
                this.servoMasterToggle.checked = enabled;
            }

            this.servoLegCards.forEach((card) => {
                card.classList.toggle("is-disabled", !enabled);
            });
        });

        const onServoState = (message: Message) => {
            if (!message.params) {
                return;
            }

            const servoId = message.getValueUint8(0);
            const control = this.servoControls.get(servoId);
            if (!control) {
                return;
            }

            const enabled = message.getValueBool(1);
            control.status.checked = enabled;
            control.min.setDisabled(!enabled);
            control.max.setDisabled(!enabled);
            control.angle.setDisabled(!enabled);
        };

        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.GET_STATE, onServoState);
        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterServoCommands.SET_STATE, onServoState);

        this.socket.addSpecificCallbackRead(ClusterName.SERVO, ClusterGenericCommands.GENERIC, (message: Message) => {
            if (!message.params) {
                return;
            }

            const servoId = message.getValueUint8(0);
            const control = this.servoControls.get(servoId);
            if (!control) {
                return;
            }

            const angle = message.getValueUint8(2);
            control.angle.setValue(angle);
            this.setReadOnlyValue(control.angleLive, `${angle} deg`);
        });
    }

    private buildUI() {
        this.root.innerHTML = "";
        this.root.classList.add("native-panel");
        this.readOnlyTraces.clear();

        const shell = this.createElement("div", "native-shell", this.root);

        const telemetrySection = this.createSection(shell, "Telemetry", false);
        this.buildTelemetry(telemetrySection);

        const bodySection = this.createSection(shell, "Body", true);
        this.buildBodySettings(bodySection);

        const legSection = this.createSection(shell, "Legs", false);
        this.buildLegSettings(legSection);

        const headSection = this.createSection(shell, "Head Sensors", false);
        this.buildHeadSensors(headSection);

        const servoSection = this.createSection(shell, "Servos", false);
        this.buildServos(servoSection);

        this.syncFromModel();
    }

    private buildTelemetry(parent: HTMLElement) {
        const batteryCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", batteryCard, "Battery");
        this.batteryVoltage = this.createReadOnlyLine(batteryCard, "Voltage", "0.00 V", true);
        this.batteryCurrent = this.createReadOnlyLine(batteryCard, "Current", "0.000 A", true);

        const imuCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", imuCard, "IMU");
        this.imuTemp = this.createReadOnlyLine(imuCard, "Temperature", "0.00 C", true,0,40);
        this.imuYaw = this.createReadOnlyLine(imuCard, "Yaw", "0.00 deg", true,0,360);
        this.imuPitch = this.createReadOnlyLine(imuCard, "Pitch", "0.00 deg", true,-180,180);
        this.imuRoll = this.createReadOnlyLine(imuCard, "Roll", "0.00 deg", true,-180,180);
        this.imuAcc = this.createReadOnlyLine(imuCard, "Accel", "0 / 0 / 0");
        this.imuGyro = this.createReadOnlyLine(imuCard, "Gyro", "0 / 0 / 0");
        this.imuMag = this.createReadOnlyLine(imuCard, "Mag", "0 / 0 / 0");

        const calibRow = this.createElement("div", "native-button-row", imuCard);
        this.createActionButton(calibRow, "Calib Acc", () => {
            this.sendIfReady(new Message(ClusterName.IMU, ClusterImuCommands.START_CALIB, [1, 1]));
        });
        this.createActionButton(calibRow, "Calib Gyro", () => {
            this.sendIfReady(new Message(ClusterName.IMU, ClusterImuCommands.START_CALIB, [2, 1]));
        });
        this.createActionButton(calibRow, "Calib Mag", () => {
            this.sendIfReady(new Message(ClusterName.IMU, ClusterImuCommands.START_CALIB, [4, 1]));
        });
    }

    private buildBodySettings(parent: HTMLElement) {
        const quickDriveCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", quickDriveCard, "Drive Control");

        const driveGrid = this.createElement("div", "native-drive-grid", quickDriveCard);
        const driveDirections: Array<{ label: string; angle: number }> = [
            { label: "NW", angle: 315 },
            { label: "N", angle: 0 },
            { label: "NE", angle: 45 },
            { label: "W", angle: 270 },
            { label: "STOP", angle: 0 },
            { label: "E", angle: 90 },
            { label: "SW", angle: 225 },
            { label: "S", angle: 180 },
            { label: "SE", angle: 135 },
        ];

        driveDirections.forEach((entry) => {
            const button = this.createActionButton(driveGrid, entry.label, () => {
                if (entry.label === "STOP") {
                    this.sendIfReady(
                        new Message(
                            ClusterName.BODY,
                            ClusterBodyCommands.SET_WALK_STATUS,
                            [2, this.hexapod.hexapodStruct.duration],
                            [0xff, 0xffff],
                        ),
                    );
                    return;
                }

                this.applyDirection(entry.angle);
            });
            button.classList.add("native-drive-button");
            if (entry.label === "STOP") {
                button.classList.add("is-stop");
            }
        });

        const walkActions = this.createElement("div", "native-drive-actions", quickDriveCard);
        this.createActionButton(walkActions, "Start Walk", () => {
            this.sendIfReady(
                new Message(
                    ClusterName.BODY,
                    ClusterBodyCommands.SET_WALK_STATUS,
                    [0, this.hexapod.hexapodStruct.duration],
                    [0xff, 0xffff],
                ),
            );
        });
        this.createActionButton(walkActions, "Stop Walk", () => {
            this.sendIfReady(
                new Message(
                    ClusterName.BODY,
                    ClusterBodyCommands.SET_WALK_STATUS,
                    [2, this.hexapod.hexapodStruct.duration],
                    [0xff, 0xffff],
                ),
            );
        });

        const rotationShortcuts = this.createElement("div", "native-rotation-shortcuts", quickDriveCard);
        const fallbackTurningRate = () => Math.max(15, this.hexapod.hexapodStruct.turningRate);
        this.createActionButton(rotationShortcuts, "↺ Left", () => {
            this.applySignedRotation(fallbackTurningRate());
        });
        this.createActionButton(rotationShortcuts, "Rotate Off", () => {
            this.applySignedRotation(0);
        });
        this.createActionButton(rotationShortcuts, "Right ↻", () => {
            this.applySignedRotation(-fallbackTurningRate());
        });

        this.createNumericControl(
            quickDriveCard,
            "Speed",
            this.speedPercentFromDuration(this.hexapod.hexapodStruct.duration),
            0,
            100,
            1,
            (value) => {
                const duration = this.durationFromSpeedPercent(value);
                this.applyDuration(duration);
            },
        );

        const gaitPills = this.createElement("div", "native-gait-pills", quickDriveCard);
        const gaitButtons = new Map<GaitOption, HTMLButtonElement>();
        const updateGaitButtons = () => {
            gaitButtons.forEach((button, gait) => {
                button.classList.toggle("is-active", this.hexapod.hexapodStruct.gait === gait);
            });
        };

        ["TRIPOD", "WAVE", "RIPPLE", "DOUBLE_WAVE"].forEach((gait) => {
            const gaitOption = gait as GaitOption;
            const button = this.createActionButton(gaitPills, gaitOption, () => {
                this.applyGait(gaitOption);
                updateGaitButtons();
            });
            button.classList.add("native-pill");
            gaitButtons.set(gaitOption, button);
        });
        updateGaitButtons();

        const movementCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", movementCard, "Body Pose");

        this.bodyPosition = this.createVectorControl(
            movementCard,
            "Position",
            this.hexapod.hexapodStruct.bodyPosition.x,
            this.hexapod.hexapodStruct.bodyPosition.y,
            this.hexapod.hexapodStruct.bodyPosition.z,
            -30,
            30,
            1,
            (x, y, z) => {
                this.hexapod.hexapodStruct.bodyPosition.x = x;
                this.hexapod.hexapodStruct.bodyPosition.y = y;
                this.hexapod.hexapodStruct.bodyPosition.z = z;
                this.sendBodyPositionRotation();
            },
        );

        this.bodyRotation = this.createVectorControl(
            movementCard,
            "Rotation",
            this.hexapod.hexapodStruct.bodyRotation.x,
            this.hexapod.hexapodStruct.bodyRotation.y,
            this.hexapod.hexapodStruct.bodyRotation.z,
            -30,
            30,
            1,
            (x, y, z) => {
                this.hexapod.hexapodStruct.bodyRotation.x = x;
                this.hexapod.hexapodStruct.bodyRotation.y = y;
                this.hexapod.hexapodStruct.bodyRotation.z = z;
                this.sendBodyPositionRotation();
            },
        );

        const gaitCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", gaitCard, "Gait & Motion");

        this.durationControl = this.createNumericControl(
            gaitCard,
            "Duration",
            this.hexapod.hexapodStruct.duration,
            1000,
            10000,
            10,
            (value) => {
                this.applyDuration(value);
            },
        );

        this.directionControl = this.createNumericControl(
            gaitCard,
            "Direction",
            this.hexapod.hexapodStruct.direction,
            0,
            360,
            1,
            (value) => {
                this.applyDirection(value);
            },
        );

        const gaitRow = this.createElement("label", "native-field", gaitCard);
        this.createElement("span", "native-field-title", gaitRow, "Gait");
        this.gaitSelect = this.createElement("select", "native-select", gaitRow) as HTMLSelectElement;
        const gaits: GaitOption[] = ["TRIPOD", "WAVE", "RIPPLE", "DOUBLE_WAVE"];
        gaits.forEach((gait) => {
            const option = this.createElement("option", "", this.gaitSelect as HTMLElement, gait) as HTMLOptionElement;
            option.value = gait;
        });

        this.gaitSelect.value = this.hexapod.hexapodStruct.gait;
        this.gaitSelect.addEventListener("change", () => {
            if (!this.gaitSelect) {
                return;
            }

            this.applyGait(this.gaitSelect.value as GaitOption);
        });

        const rotationCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", rotationCard, "Rotation (↺ Left  •  ↻ Right)");

        this.rotationControl = this.createNumericControl(
            rotationCard,
            "Turn rate",
            this.signedRotationFromState(),
            -180,
            180,
            1,
            (value) => {
                this.applySignedRotation(value);
            },
        );

        const profileCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", profileCard, "Step Profile");

        this.amplitudeControl = this.createNumericControl(
            profileCard,
            "Amplitude",
            this.hexapod.hexapodStruct.amplitude,
            0,
            30,
            1,
            (value) => {
                this.hexapod.hexapodStruct.amplitude = value;
                this.sendIfReady(new Message(ClusterName.BODY, ClusterBodyCommands.SET_AMPLITUDE, [value]));
            },
        );

        this.elevationControl = this.createNumericControl(
            profileCard,
            "Elevation",
            this.hexapod.hexapodStruct.elevation,
            0,
            30,
            1,
            (value) => {
                this.hexapod.hexapodStruct.elevation = value;
                this.sendIfReady(new Message(ClusterName.BODY, ClusterBodyCommands.SET_ELEVATION, [value]));
            },
        );
    }

    private buildLegSettings(parent: HTMLElement) {
        for (let i = 0; i < 6; i++) {
            const leg = this.hexapod.body.members.legs.leg[i];
            const card = this.createElement("div", "native-card", parent);
            this.createElement("h4", "native-card-title", card, leg.legData.name);

            const vector = this.createVectorControl(
                card,
                "Target",
                leg.x,
                leg.y,
                leg.z,
                -30,
                30,
                1,
                (x, y, z) => {
                    leg.x = x;
                    leg.y = y;
                    leg.z = z;
                    this.sendLegPosition(i);
                },
            );

            this.legVectors.push(vector);
        }
    }

    private buildHeadSensors(parent: HTMLElement) {
        const proximityCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", proximityCard, "Proximity");
        this.proximityLeft = this.createReadOnlyLine(proximityCard, "Left", "0", true);
        this.proximityFront = this.createReadOnlyLine(proximityCard, "Front max", "0", true);
        this.proximityRight = this.createReadOnlyLine(proximityCard, "Right", "0", true);

        const proximityButtons = this.createElement("div", "native-button-row", proximityCard);
        this.createActionButton(proximityButtons, "Read Left", () => {
            this.sendIfReady(new Message(ClusterName.PROXIMITY, ClusterProximityCommands.US_LEFT));
        });
        this.createActionButton(proximityButtons, "Read Front", () => {
            this.sendIfReady(new Message(ClusterName.PROXIMITY, ClusterProximityCommands.LASER));
        });
        this.createActionButton(proximityButtons, "Read Right", () => {
            this.sendIfReady(new Message(ClusterName.PROXIMITY, ClusterProximityCommands.US_RIGHT));
        });
        // Add reset button for proximity trace
        this.createActionButton(proximityButtons, "Reset Proximity Trace", () => {
            if (this.hexapod && this.hexapod.head && this.hexapod.head.proximity) {
                this.hexapod.head.proximity.resetTraces();
            }
        });

        const soundCard = this.createElement("div", "native-card", parent);
        this.createElement("h4", "native-card-title", soundCard, "Sound");
        this.soundLeft = this.createReadOnlyLine(soundCard, "Left", "0", true);
        this.soundRight = this.createReadOnlyLine(soundCard, "Right", "0", true);

        const soundButtons = this.createElement("div", "native-button-row", soundCard);
        this.createActionButton(soundButtons, "Read Left", () => {
            this.sendIfReady(new Message(ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, [0]));
        });
        this.createActionButton(soundButtons, "Read Right", () => {
            this.sendIfReady(new Message(ClusterName.SOUND, ClusterSoundCommands.SOUND_STATUS, [1]));
        });
    }

    private buildServos(parent: HTMLElement) {
        const masterCard = this.createElement("div", "native-card", parent);
        const masterToggle = this.createElement("label", "native-toggle", masterCard);
        this.servoMasterToggle = this.createElement("input", "", masterToggle) as HTMLInputElement;
        this.servoMasterToggle.type = "checkbox";
        this.servoMasterToggle.checked = this.hexapod.body.members.legs.status;
        this.createElement("span", "native-toggle-label", masterToggle, "Enable all servos (PCA)");

        this.servoMasterToggle.addEventListener("change", () => {
            if (!this.servoMasterToggle) {
                return;
            }

            this.hexapod.body.members.legs.status = this.servoMasterToggle.checked;
            this.sendIfReady(
                new Message(
                    ClusterName.SERVO,
                    ClusterServoCommands.SET_STATE_PCA,
                    [this.servoMasterToggle.checked ? 1 : 0],
                ),
            );
        });

        for (let i = 0; i < 6; i++) {
            const leg = this.hexapod.body.members.legs.leg[i];

            const legCard = this.createElement("details", "native-card native-servo-leg", parent) as HTMLDetailsElement;
            legCard.open = false;
            this.createElement("summary", "native-card-title", legCard, leg.legData.name);

            this.servoLegCards.push(legCard);

            for (let j = 0; j < 3; j++) {
                const servoId = i * 3 + j;
                const servoData = leg.legData.servos[j];
                const servoCard = this.createElement("details", "native-servo-card", legCard) as HTMLDetailsElement;
                servoCard.open = false;
                this.createElement("summary", "native-servo-title", servoCard, servoData.name);

                const statusRow = this.createElement("label", "native-toggle", servoCard);
                const status = this.createElement("input", "", statusRow) as HTMLInputElement;
                status.type = "checkbox";
                status.checked = servoData.status;
                this.createElement("span", "native-toggle-label", statusRow, "Enabled");

                status.addEventListener("change", () => {
                    servoData.status = status.checked;
                    this.sendIfReady(
                        new Message(
                            ClusterName.SERVO,
                            ClusterServoCommands.SET_STATE,
                            [servoId, status.checked ? 1 : 0],
                        ),
                    );
                });

                const min = this.createNumericControl(servoCard, "Min", servoData.min, 0, 180, 1, (value) => {
                    servoData.min = value;
                    this.sendIfReady(new Message(ClusterName.SERVO, ClusterServoCommands.SET_MIN, [servoId, value]));
                });

                const max = this.createNumericControl(servoCard, "Max", servoData.max, 0, 180, 1, (value) => {
                    servoData.max = value;
                    this.sendIfReady(new Message(ClusterName.SERVO, ClusterServoCommands.SET_MAX, [servoId, value]));
                });

                const angle = this.createNumericControl(servoCard, "Angle", servoData.angle, 0, 180, 1, (value) => {
                    servoData.angle = value;
                    this.sendIfReady(new Message(ClusterName.SERVO, ClusterServoCommands.SET_ANGLE, [servoId, value]));
                });

                const angleLive = this.createReadOnlyLine(servoCard, "Live", `${servoData.angle} deg`, true, parseInt(min.number.value), parseInt(max.number.value));

                this.servoControls.set(servoId, {
                    status,
                    min,
                    max,
                    angle,
                    angleLive,
                });
            }
        }
    }

    private syncFromModel() {
        if (this.bodyPosition) {
            this.bodyPosition.x.setValue(this.hexapod.hexapodStruct.bodyPosition.x);
            this.bodyPosition.y.setValue(this.hexapod.hexapodStruct.bodyPosition.y);
            this.bodyPosition.z.setValue(this.hexapod.hexapodStruct.bodyPosition.z);
        }

        if (this.bodyRotation) {
            this.bodyRotation.x.setValue(this.hexapod.hexapodStruct.bodyRotation.x);
            this.bodyRotation.y.setValue(this.hexapod.hexapodStruct.bodyRotation.y);
            this.bodyRotation.z.setValue(this.hexapod.hexapodStruct.bodyRotation.z);
        }

        if (this.durationControl) {
            this.durationControl.setValue(this.hexapod.hexapodStruct.duration);
        }

        if (this.directionControl) {
            this.directionControl.setValue(this.hexapod.hexapodStruct.direction);
        }

        if (this.gaitSelect) {
            this.gaitSelect.value = this.hexapod.hexapodStruct.gait;
        }

        if (this.rotationControl) {
            this.rotationControl.setValue(this.signedRotationFromState());
        }

        if (this.amplitudeControl) {
            this.amplitudeControl.setValue(this.hexapod.hexapodStruct.amplitude);
        }

        if (this.elevationControl) {
            this.elevationControl.setValue(this.hexapod.hexapodStruct.elevation);
        }

        this.legVectors.forEach((vector, index) => {
            const leg = this.hexapod.body.members.legs.leg[index];
            vector.x.setValue(leg.x);
            vector.y.setValue(leg.y);
            vector.z.setValue(leg.z);
        });

        if (this.batteryVoltage) {
            this.setReadOnlyValue(this.batteryVoltage, `${this.hexapod.battery.batteryData.voltage.toFixed(2)} V`);
        }
        if (this.batteryCurrent) {
            this.setReadOnlyValue(this.batteryCurrent, `${this.hexapod.battery.batteryData.current.toFixed(3)} A`);
        }

        if (this.imuTemp) {
            this.setReadOnlyValue(this.imuTemp, `${this.hexapod.imu.imuData.temperature.toFixed(2)} C`);
        }
        if (this.imuYaw) {
            this.setReadOnlyValue(this.imuYaw, `${this.hexapod.imu.imuData.ypr.yaw.toFixed(2)} deg`);
        }
        if (this.imuPitch) {
            this.setReadOnlyValue(this.imuPitch, `${this.hexapod.imu.imuData.ypr.pitch.toFixed(2)} deg`);
        }
        if (this.imuRoll) {
            this.setReadOnlyValue(this.imuRoll, `${this.hexapod.imu.imuData.ypr.roll.toFixed(2)} deg`);
        }
        if (this.imuAcc) {
            this.setReadOnlyValue(
                this.imuAcc,
                `${this.hexapod.imu.imuData.accel.x} / ${this.hexapod.imu.imuData.accel.y} / ${this.hexapod.imu.imuData.accel.z}`,
            );
        }
        if (this.imuGyro) {
            this.setReadOnlyValue(
                this.imuGyro,
                `${this.hexapod.imu.imuData.gyro.x} / ${this.hexapod.imu.imuData.gyro.y} / ${this.hexapod.imu.imuData.gyro.z}`,
            );
        }
        if (this.imuMag) {
            this.setReadOnlyValue(
                this.imuMag,
                `${this.hexapod.imu.imuData.mag.x} / ${this.hexapod.imu.imuData.mag.y} / ${this.hexapod.imu.imuData.mag.z}`,
            );
        }

        if (this.proximityLeft) {
            this.setReadOnlyValue(this.proximityLeft, this.hexapod.head.sensors.proximity.left.toString());
        }
        if (this.proximityRight) {
            this.setReadOnlyValue(this.proximityRight, this.hexapod.head.sensors.proximity.right.toString());
        }
        if (this.proximityFront) {
            const front = this.hexapod.head.sensors.proximity.front;
            let maxFront = 0;
            for (let row = 0; row < front.length; row++) {
                for (let col = 0; col < front[row].length; col++) {
                    maxFront = Math.max(maxFront, front[row][col]);
                }
            }
            this.setReadOnlyValue(this.proximityFront, maxFront.toFixed(1));
        }

        if (this.soundLeft) {
            this.setReadOnlyValue(this.soundLeft, this.hexapod.head.sensors.sound.left.toString());
        }
        if (this.soundRight) {
            this.setReadOnlyValue(this.soundRight, this.hexapod.head.sensors.sound.right.toString());
        }

        if (this.servoMasterToggle) {
            this.servoMasterToggle.checked = this.hexapod.body.members.legs.status;
        }

        this.servoControls.forEach((control, servoId) => {
            const legIndex = Math.floor(servoId / 3);
            const servoIndex = servoId % 3;
            const servoData = this.hexapod.body.members.legs.leg[legIndex].legData.servos[servoIndex];

            control.status.checked = servoData.status;
            control.min.setValue(servoData.min);
            control.max.setValue(servoData.max);
            control.angle.setValue(servoData.angle);
            this.setReadOnlyValue(control.angleLive, `${servoData.angle} deg`, parseInt(control.min.number.value), parseInt(control.max.number.value));
        });
    }

    private sendBodyPositionRotation() {
        this.sendIfReady(
            new Message(
                ClusterName.BODY,
                ClusterBodyCommands.SET_BODY_X_Y_Z,
                [
                    Math.floor(this.hexapod.hexapodStruct.bodyPosition.x),
                    Math.floor(this.hexapod.hexapodStruct.bodyPosition.y),
                    Math.floor(this.hexapod.hexapodStruct.bodyPosition.z),
                    Math.floor(this.hexapod.hexapodStruct.bodyRotation.x),
                    Math.floor(this.hexapod.hexapodStruct.bodyRotation.y),
                    Math.floor(this.hexapod.hexapodStruct.bodyRotation.z),
                    BODY_DURATION_MS,
                ],
                [0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff],
            ),
        );
    }

    private applyDuration(duration: number) {
        const clamped = Math.max(MIN_DURATION_MS, Math.min(MAX_DURATION_MS, Math.round(duration)));
        this.hexapod.hexapodStruct.duration = clamped;

        if (this.durationControl) {
            this.durationControl.setValue(clamped);
        }

        this.sendIfReady(new Message(ClusterName.BODY, ClusterBodyCommands.SET_DURATION, [clamped], [0xffff]));
    }

    private applyDirection(direction: number) {
        const normalized = ((Math.round(direction) % 360) + 360) % 360;
        this.hexapod.hexapodStruct.direction = normalized;
        this.hexapod.setDirection(normalized);

        if (this.directionControl) {
            this.directionControl.setValue(normalized);
        }

        this.sendIfReady(new Message(ClusterName.BODY, ClusterBodyCommands.SET_DIRECTION, [normalized], [0xffff]));
    }

    private applyGait(gait: GaitOption) {
        this.hexapod.hexapodStruct.gait = gait;

        if (this.gaitSelect) {
            this.gaitSelect.value = gait;
        }

        const gaitCode = gait === "TRIPOD" ? 0 : gait === "WAVE" ? 1 : gait === "RIPPLE" ? 2 : 3;
        this.sendIfReady(new Message(ClusterName.BODY, ClusterBodyCommands.SET_GAIT, [gaitCode]));
    }

    private applyRotation(rotation: number, clockwise: boolean) {
        const clamped = Math.max(0, Math.min(360, Math.round(rotation)));
        this.hexapod.setRotation(clamped);
        this.hexapod.setRotationClockwise(clockwise);

        if (this.rotationControl) {
            this.rotationControl.setValue(this.signedRotationFromState());
        }

        this.sendIfReady(
            new Message(
                ClusterName.BODY,
                ClusterBodyCommands.SET_ROTATION,
                [clamped, clockwise ? 1 : 0],
                [0xffff, 0xff],
            ),
        );
    }

    /**
     * Apply a signed rotation request from the unified UI control.
     *
     * Sign convention: positive value = CCW (turn left), negative = CW (turn right).
     */
    private applySignedRotation(signedDeg: number) {
        const clamped = Math.max(-180, Math.min(180, Math.round(signedDeg)));
        const magnitude = Math.abs(clamped);
        const clockwise = clamped < 0;
        this.applyRotation(magnitude, clockwise);
    }

    private signedRotationFromState(): number {
        const magnitude = this.hexapod.hexapodStruct.turningRate;
        return this.hexapod.hexapodStruct.clockwise ? -magnitude : magnitude;
    }

    private speedPercentFromDuration(duration: number): number {
        const clamped = Math.max(MIN_DURATION_MS, Math.min(MAX_DURATION_MS, duration));
        return Math.round(((MAX_DURATION_MS - clamped) / (MAX_DURATION_MS - MIN_DURATION_MS)) * 100);
    }

    private durationFromSpeedPercent(speedPercent: number): number {
        const clamped = Math.max(0, Math.min(100, Math.round(speedPercent)));
        return Math.round(MAX_DURATION_MS - ((MAX_DURATION_MS - MIN_DURATION_MS) * clamped) / 100);
    }

    private sendLegPosition(legIndex: number) {
        const leg = this.hexapod.body.members.legs.leg[legIndex];
        this.sendIfReady(
            new Message(
                ClusterName.BODY,
                ClusterBodyCommands.SET_LEG_X_Y_Z,
                [legIndex, Math.floor(leg.x), Math.floor(leg.y), Math.floor(leg.z), BODY_DURATION_MS],
                [0xff, 0xffff, 0xffff, 0xffff, 0xffff],
            ),
        );
    }

    private sendIfReady(message: Message) {
        if (!this.initDone) {
            return;
        }
        this.socket.write(message);
    }

    private createSection(parent: HTMLElement, title: string, open: boolean): HTMLElement {
        const section = this.createElement("details", "native-section", parent) as HTMLDetailsElement;
        section.open = open;
        this.createElement("summary", "native-section-title", section, title);
        return this.createElement("div", "native-section-body", section);
    }

    private createVectorControl(
        parent: HTMLElement,
        title: string,
        x: number,
        y: number,
        z: number,
        min: number,
        max: number,
        step: number,
        onCommit: (nextX: number, nextY: number, nextZ: number) => void,
    ): LegVectorControl {
        const wrapper = this.createElement("div", "native-group", parent);
        this.createElement("h5", "native-group-title", wrapper, title);

        const control: LegVectorControl = {
            x: this.createNumericControl(wrapper, "X", x, min, max, step, () => {
                onCommit(control.x.number.valueAsNumber, control.y.number.valueAsNumber, control.z.number.valueAsNumber);
            }),
            y: this.createNumericControl(wrapper, "Y", y, min, max, step, () => {
                onCommit(control.x.number.valueAsNumber, control.y.number.valueAsNumber, control.z.number.valueAsNumber);
            }),
            z: this.createNumericControl(wrapper, "Z", z, min, max, step, () => {
                onCommit(control.x.number.valueAsNumber, control.y.number.valueAsNumber, control.z.number.valueAsNumber);
            }),
        };

        return control;
    }

    private createNumericControl(
        parent: HTMLElement,
        label: string,
        initialValue: number,
        min: number,
        max: number,
        step: number,
        onCommit: (value: number) => void,
    ): NumericControl {
        const row = this.createElement("div", "native-field", parent);
        const title = this.createElement("label", "native-field-title", row, label);
        const value = this.createElement("span", "native-field-value", title, initialValue.toString());

        const controlRow = this.createElement("div", "native-input-row", row);
        const range = this.createElement("input", "native-range", controlRow) as HTMLInputElement;
        range.type = "range";
        range.min = min.toString();
        range.max = max.toString();
        range.step = step.toString();

        const number = this.createElement("input", "native-number", controlRow) as HTMLInputElement;
        number.type = "number";
        number.min = min.toString();
        number.max = max.toString();
        number.step = step.toString();

        const setValue = (next: number) => {
            const clamped = Math.min(max, Math.max(min, next));
            range.value = clamped.toString();
            number.value = clamped.toString();
            value.innerText = clamped.toString();
        };

        const setDisabled = (disabled: boolean) => {
            range.disabled = disabled;
            number.disabled = disabled;
            row.classList.toggle("is-disabled", disabled);
        };

        setValue(initialValue);

        range.addEventListener("input", () => {
            const next = Number(range.value);
            number.value = next.toString();
            value.innerText = next.toString();
        });

        range.addEventListener("change", () => {
            const next = Number(range.value);
            setValue(next);
            onCommit(next);
        });

        number.addEventListener("change", () => {
            const next = Number(number.value);
            setValue(next);
            onCommit(Number(number.value));
        });

        return {
            range,
            number,
            value,
            setValue,
            setDisabled,
        };
    }

    private setReadOnlyValue(target: HTMLElement | null, value: string, min?: number, max?: number) {
        if (!target) {
            return;
        }

        target.innerText = value;
        const trace = this.readOnlyTraces.get(target);
        if (!trace) {
            return;
        }

        trace.minValue = min !== undefined ? min : Math.min(trace.minValue, trace.samples.length > 0 ? trace.samples[trace.samples.length - 1].v : Number.POSITIVE_INFINITY);
        trace.maxValue = max !== undefined ? max : Math.max(trace.maxValue, trace.samples.length > 0 ? trace.samples[trace.samples.length - 1].v : Number.NEGATIVE_INFINITY);

        const parsedValue = Number.parseFloat(value);
        if (!Number.isFinite(parsedValue)) {
            return;
        }

        const now = performance.now();
        if (now - trace.lastSampleAt < TRACE_SAMPLE_INTERVAL_MS) {
            return;
        }

        trace.lastSampleAt = now;
        trace.samples.push({
            t: now,
            v: parsedValue,
        });

        const minTime = now - TRACE_WINDOW_MS;
        while (trace.samples.length > 0 && trace.samples[0].t < minTime) {
            trace.samples.shift();
        }

        this.drawReadOnlyTrace(trace, now);
    }

    private drawReadOnlyTrace(trace: ReadOnlyTrace, now: number) {
        const context = trace.canvas.getContext("2d");
        if (!context) {
            return;
        }

        const dpr = window.devicePixelRatio || 1;
        const cssWidth = Math.max(120, Math.floor(trace.canvas.clientWidth || 120));
        const cssHeight = Math.max(28, Math.floor(trace.canvas.clientHeight || 28));
        const pxWidth = Math.floor(cssWidth * dpr);
        const pxHeight = Math.floor(cssHeight * dpr);
        if (trace.canvas.width !== pxWidth || trace.canvas.height !== pxHeight) {
            trace.canvas.width = pxWidth;
            trace.canvas.height = pxHeight;
        }

        context.setTransform(dpr, 0, 0, dpr, 0, 0);
        context.clearRect(0, 0, cssWidth, cssHeight);
        if (trace.samples.length === 0) {
            return;
        }

        let minValue = trace.samples[0].v;
        let maxValue = trace.samples[0].v;
        trace.samples.forEach((sample) => {
            minValue = Math.min(trace.minValue, sample.v);
            maxValue = Math.max(trace.maxValue, sample.v);
        });

        const valueSpan = Math.max(0.0001, maxValue - minValue);
        const startTime = now - TRACE_WINDOW_MS;
        const width = cssWidth - 4;
        const height = cssHeight - 4;

        context.strokeStyle = "rgba(122, 246, 213, 0.9)";
        context.lineWidth = 1.4;
        context.beginPath();

        trace.samples.forEach((sample, index) => {
            const normalizedTime = Math.max(0, Math.min(1, (sample.t - startTime) / TRACE_WINDOW_MS));
            const normalizedValue = (sample.v - minValue) / valueSpan;
            const x = 2 + normalizedTime * width;
            const y = 2 + (1 - normalizedValue) * height;
            if (index === 0) {
                context.moveTo(x, y);
                return;
            }
            context.lineTo(x, y);
        });

        context.stroke();
    }

    private createReadOnlyLine(parent: HTMLElement, label: string, value: string, withTrace: boolean = false, min?: number, max?: number): HTMLElement {
        const line = this.createElement("div", "native-readonly", parent);
        this.createElement("span", "native-readonly-label", line, label);
        const valueNode = this.createElement("span", "native-readonly-value", line, value);

        if (withTrace) {
            const trace = this.createElement("canvas", "native-readonly-trace", line) as HTMLCanvasElement;
            this.readOnlyTraces.set(valueNode, {
                canvas: trace,
                samples: [],
                lastSampleAt: Number.NEGATIVE_INFINITY,
                minValue: min !== undefined ? min : Number.POSITIVE_INFINITY,
                maxValue: max !== undefined ? max : Number.NEGATIVE_INFINITY,
            });
        }

        return valueNode;
    }

    private createActionButton(parent: HTMLElement, label: string, onClick: () => void) {
        const button = this.createElement("button", "native-button", parent, label) as HTMLButtonElement;
        button.type = "button";
        button.addEventListener("click", onClick);
        return button;
    }

    private createElement<K extends keyof HTMLElementTagNameMap>(
        tag: K,
        className: string,
        parent: HTMLElement,
        text?: string,
    ): HTMLElementTagNameMap[K] {
        const element = document.createElement(tag);
        if (className) {
            element.className = className;
        }
        if (text !== undefined) {
            element.innerText = text;
        }
        parent.appendChild(element);
        return element;
    }
}
