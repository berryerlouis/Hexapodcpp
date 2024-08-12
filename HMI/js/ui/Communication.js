import SerialInterface from '../protocol/Serial.js'
import { Message } from '../protocol/Message.js'
import * as Cluters from '../protocol/Cluster.js';

export default class Communication {
    consoleList = undefined;
    constructor(console, canvas) {
        this.uiCanvas = canvas;
        this.uiConsole = console;
        this.uiConsole.sendRawMessage(this.sendRawMessage.bind(this));
        this.connectButton = $('#connect-button');
        this.serialInterface = new SerialInterface();
        this.messages = [];
        this.currentSentMessage = null;


        this.connectButton.click(async () => {
            await this.serialInterface.init();
            setInterval(() => {
                //RX part
                if (this.serialInterface.MessageAvailable()) {
                    const frame = this.serialInterface.PopMessage();
                    if (frame.raw == "<hello>") {
                        this.currentSentMessage = null;
                        this.timeout = 0;
                        this.uiConsole.log("RESET", frame, false)
                    }
                    else if (frame.raw == "<ffff0101>") {
                        this.currentSentMessage = null;
                        this.timeout = 0;
                        this.uiConsole.log("ERROR", { raw }, false)
                    }
                    else {
                        if (this.currentSentMessage
                            && ((frame.cluster.code == this.currentSentMessage.cluster.code)
                                && (frame.command.code == this.currentSentMessage.command.code))) {
                            this.currentSentMessage = null;
                            this.timeout = 0;
                            this.uiConsole.log("RX", frame, true)
                        }
                        else {
                            this.uiConsole.log("RX", frame, false)
                        }
                        this.executeMessage(frame);
                    }
                }

                //TX part
                if (this.messages.length > 0 && this.currentSentMessage == null) {
                    this.currentSentMessage = this.messages.pop();
                    this.serialInterface.write(this.currentSentMessage.raw);
                    this.uiConsole.log("TX", this.currentSentMessage, false)
                }
                else if (this.currentSentMessage != null) {
                    this.timeout++;
                    if (this.timeout > 20) {
                        this.uiConsole.log("TIMEOUT", this.currentSentMessage, false)
                        this.currentSentMessage = null;
                        this.timeout = 0;
                    }
                }
            }, 1)


            this.sendRawMessage(Cluters.ClusterName.GENERAL, Cluters.CommandGeneral.VERSION)
            this.sendRawMessage(Cluters.ClusterName.BATTERY, Cluters.CommandBattery.STATUS)

            setInterval(() => {
                this.sendRawMessage(Cluters.ClusterName.BATTERY, Cluters.CommandBattery.STATUS)
            }, 10 * 1000)

            setInterval(() => {
                this.sendRawMessage(Cluters.ClusterName.SERVO, Cluters.CommandServo.GET_ALL)
            }, 50)

            setInterval(() => {
                this.sendRawMessage(Cluters.ClusterName.IMU, Cluters.CommandImu.ACC)
                this.sendRawMessage(Cluters.ClusterName.IMU, Cluters.CommandImu.GYR)
                this.sendRawMessage(Cluters.ClusterName.IMU, Cluters.CommandImu.MAG)
            }, 1000)
        });
    }

    sendMessage(msg) {
        this.messages.push(msg)
    }

    sendRawMessage(cluster = undefined, command = undefined) {
        this.messages.push(new Message().build(cluster, command))
    }

    executeMessage(message) {
        try {

            if (message.cluster.name === Cluters.ClusterName.SERVO) {
                if (message.command.name === Cluters.CommandServo.GET_ALL) {
                    if (message.size !== 0) {
                        for (let servoId = 0; servoId < message.params.length; servoId++) {
                            let angleServo = message.fetchInt8U() - 90;
                            this.uiCanvas.moveServo(servoId, angleServo);
                        }
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.IMU) {
                if (message.command.name === Cluters.CommandImu.ACC) {
                    if (message.size !== 0) {
                        $('#accel-x').text(message.fetchInt16S());
                        $('#accel-y').text(message.fetchInt16S());
                        $('#accel-z').text(message.fetchInt16S());
                    }
                }
                else if (message.command.name === Cluters.CommandImu.GYR) {
                    if (message.size !== 0) {
                        $('#gyro-x').text(message.fetchInt16S());
                        $('#gyro-y').text(message.fetchInt16S());
                        $('#gyro-z').text(message.fetchInt16S());
                    }
                }
                else if (message.command.name === Cluters.CommandImu.MAG) {
                    if (message.size !== 0) {
                        $('#mag-x').text(message.fetchInt16S());
                        $('#mag-y').text(message.fetchInt16S());
                        $('#mag-z').text(message.fetchInt16S());
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.PROXIMITY) {
                if (message.command.name === Cluters.CommandProximity.US_LEFT) {
                    if (message.size > 1) {
                        let distance = message.fetchInt16U();
                        this.uiCanvas.drawObstacleLeft(distance);
                    }
                }
                else if (message.command.name === Cluters.CommandProximity.LAZER) {
                    if (message.size > 1) {
                        let distance = message.fetchInt16U();
                        this.uiCanvas.drawObstacleCenter(distance / 10);
                    }
                }
                else if (message.command.name === Cluters.CommandProximity.US_RIGHT) {
                    if (message.size > 1) {
                        let distance = message.fetchInt16U();
                        this.uiCanvas.drawObstacleRight(distance);
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.GENERAL) {
                if (message.command.name === Cluters.CommandGeneral.VERSION) {
                    if (message.size > 0) {
                        let major = message.fetchInt8U();
                        let minor = message.fetchInt8U();
                        $('#hexapod-version').text(major + '.' + minor);
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.BATTERY) {
                if (message.command.name === Cluters.CommandBattery.STATUS) {
                    if (message.size > 0) {
                        let state = message.fetchInt8U();
                        let voltage = message.fetchInt16U() / 100;
                        if (state == 0) {
                            $('#hexapod-battery-status').removeClass('bi-battery');
                            $('#hexapod-battery-status').removeClass('bi-battery-half');
                            $('#hexapod-battery-status').addClass('bi-battery-full');
                            $('#hexapod-battery-status').attr('style', "color: rgb(50, 223, 27);");
                        }
                        else if (state == 1) {
                            $('#hexapod-battery-status').removeClass('bi-battery');
                            $('#hexapod-battery-status').removeClass('bi-battery-full');
                            $('#hexapod-battery-status').addClass('bi-battery-half');
                            $('#hexapod-battery-status').attr('style', "color: rgb(223, 135, 27);");
                        }
                        else if (state == 2) {
                            $('#hexapod-battery-status').removeClass('bi-battery-full');
                            $('#hexapod-battery-status').removeClass('bi-battery-half');
                            $('#hexapod-battery-status').addClass('bi-battery');
                            $('#hexapod-battery-status').attr('style', "color: rgb(223, 27, 27);");
                        }
                        else {
                            $('#hexapod-battery-status').removeClass('bi-battery-full');
                            $('#hexapod-battery-status').removeClass('bi-battery-half');
                            $('#hexapod-battery-status').addClass('bi-battery');
                            $('#hexapod-battery-status').attr('style', "color: rgb(223, 27, 27);");
                        }
                        $('#hexapod-battery-voltage').text(voltage.toFixed(2) + 'V');
                    }
                }
            }
        }
        catch {
            console.error(message.raw)
        }
    }
}