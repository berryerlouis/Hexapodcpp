import SerialInterface from '../protocol/Serial.js'
import { Message } from '../protocol/Message.js'
import * as Cluters from '../protocol/Cluster.js';
import { Protocol } from '../protocol/Protocol.js';

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
                if (this.serialInterface.dataAvailable()) {
                    const data = this.serialInterface.read();
                    if (data.indexOf('<') != -1 && data.indexOf('>') != -1) {
                        let raw = data.substring(data.indexOf('<'), data.indexOf('>') + 1);
                        try {
                            if (raw == "<hello>") {
                                this.currentSentMessage = null;
                                this.timeout = 0;
                                this.uiConsole.log("RESET", frame, false)
                            }
                            else if (raw == "<ffff0101>") {
                                this.currentSentMessage = null;
                                this.timeout = 0;
                                this.uiConsole.log("ERROR", { raw }, false)
                            }
                            else {
                                let frame = Protocol.decode(raw);

                                this.executeMessage(frame);

                                if (this.currentSentMessage && (frame.cluster.code == this.currentSentMessage.cluster.code &&
                                    frame.command.code == this.currentSentMessage.command.code)) {
                                    this.currentSentMessage = null;
                                    this.timeout = 0;
                                    this.uiConsole.log("RX", frame, true)
                                }
                                else {
                                    this.uiConsole.log("RX", frame, false)
                                }
                            }
                        }
                        catch (msg) {
                            this.currentSentMessage = null;
                            this.timeout = 0;
                            this.uiConsole.log("RX", { raw }, false)
                        }
                        this.serialInterface.eat(raw);
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
            }, 10)

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
                            let angleServo = parseInt(message.params[servoId], 16) - 90;
                            this.uiCanvas.moveServo(servoId, angleServo);
                        }
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.IMU) {
                if (message.command.name === Cluters.CommandImu.ACC) {
                    if (message.size !== 0) {
                        let t = parseInt(message.params[1] + message.params[0], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#accel-x').text(t);
                        t = parseInt(message.params[3] + message.params[2], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#accel-y').text(t);
                        t = parseInt(message.params[5] + message.params[4], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#accel-z').text(t);
                    }
                }
                else if (message.command.name === Cluters.CommandImu.GYR) {
                    if (message.size !== 0) {
                        let t = parseInt(message.params[1] + message.params[0], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#gyro-x').text(t);
                        t = parseInt(message.params[3] + message.params[2], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#gyro-y').text(t);
                        t = parseInt(message.params[5] + message.params[4], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#gyro-z').text(t);
                    }
                }
                else if (message.command.name === Cluters.CommandImu.MAG) {
                    if (message.size !== 0) {
                        let t = parseInt(message.params[1] + message.params[0], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#mag-x').text(t);
                        t = parseInt(message.params[3] + message.params[2], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#mag-y').text(t);
                        t = parseInt(message.params[5] + message.params[4], 16)
                        t = (t & 0x8000) > 0 ? t - 0x10000 : t
                        $('#mag-z').text(t);
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.PROXIMITY) {
                if (message.command.name === Cluters.CommandProximity.US_LEFT) {
                    if (message.size > 1) {
                        this.uiCanvas.drawObstacleLeft(parseInt(message.params[1], 16) * 16 + parseInt(message.params[0], 16));
                    }
                }
                else if (message.command.name === Cluters.CommandProximity.LAZER) {
                    if (message.size > 1) {
                        this.uiCanvas.drawObstacleCenter((parseInt(message.params[1], 16) * 16 + parseInt(message.params[0], 16)) / 10);
                    }
                }
                else if (message.command.name === Cluters.CommandProximity.US_RIGHT) {
                    if (message.size > 1) {
                        this.uiCanvas.drawObstacleRight(parseInt(message.params[1], 16) * 16 + parseInt(message.params[0], 16));
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.GENERAL) {
                if (message.command.name === Cluters.CommandGeneral.VERSION) {
                    if (message.size > 0) {
                        $('#hexapod-version').text(message.params[0] + '.' + message.params[1]);
                    }
                }
            }
            else if (message.cluster.name === Cluters.ClusterName.BATTERY) {
                if (message.command.name === Cluters.CommandBattery.STATUS) {
                    if (message.size > 0) {
                        if (message.params[0] == 0) {
                            $('#hexapod-battery-status').removeClass('bi-battery');
                            $('#hexapod-battery-status').removeClass('bi-battery-half');
                            $('#hexapod-battery-status').addClass('bi-battery-full');
                            $('#hexapod-battery-status').attr('style', "color: rgb(50, 223, 27);");
                        }
                        else if (message.params[0] == 1) {
                            $('#hexapod-battery-status').removeClass('bi-battery');
                            $('#hexapod-battery-status').removeClass('bi-battery-full');
                            $('#hexapod-battery-status').addClass('bi-battery-half');
                            $('#hexapod-battery-status').attr('style', "color: rgb(223, 135, 27);");
                        }
                        else if (message.params[0] == 2) {
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
                        $('#hexapod-battery-voltage').text(((parseInt(message.params[1] + message.params[2], 16)) / 100).toFixed(2) + 'V');
                    }
                }
            }
        }
        catch {
            console.error(message.raw)
        }
    }
}