import * as Cluters from '../protocol/Cluster.js';
import { Message } from '../protocol/Message.js'

export default class Command {
    constructor(communication) {
        this.communication = communication;
        this.button = $('#open-command');
        this.close = $('#command-menu-close');
        this.menu = $('#command-menu');

        this.positionX = $('#body-position-x');
        this.positionY = $('#body-position-y');
        this.positionZ = $('#body-position-z');
        this.positionX[0].onchange = (() => { this.updateBody(); })
        this.positionY[0].onchange = (() => { this.updateBody(); })
        this.positionZ[0].onchange = (() => { this.updateBody(); })
        this.rotationX = $('#body-rotation-x');
        this.rotationY = $('#body-rotation-y');
        this.rotationZ = $('#body-rotation-z');
        this.rotationX[0].onchange = (() => { this.updateBody(); })
        this.rotationY[0].onchange = (() => { this.updateBody(); })
        this.rotationZ[0].onchange = (() => { this.updateBody(); })


        this.servoId = $('#servo-id');
        this.servoAngle = $('#servo-angle');
        this.servoAngle[0].onchange = (() => { this.updateServoAngle(); })
        this.servoOffset = $('#servo-offset');
        this.servoOffset[0].onchange = (() => { this.updateServoOffset(); })

        this.close.click(async () => {
            this.menu.hide();
        });
        this.button.click(async () => {
            this.menu.show();
        });
        this.menu.show();


    }

    updateServoAngle() {
        $('#servo-angle-value')[0].innerText = this.servoAngle[0].value
        let msg = new Message().build(Cluters.ClusterName.SERVO, Cluters.CommandServo.SET_ANGLE, 2, [
            parseInt(this.servoId[0].value), this.decimalTo8BitsHexString(parseInt(this.servoAngle[0].value))
        ]);
        this.communication.sendMessage(msg)
    }

    updateServoOffset() {
        $('#servo-offset-value')[0].innerText = this.servoOffset[0].value
        let msg = new Message().build(Cluters.ClusterName.SERVO, Cluters.CommandServo.SET_OFFSET, 2, [
            parseInt(this.servoId[0].value), this.decimalTo8BitsHexString(parseInt(this.servoOffset[0].value))
        ]);
        this.communication.sendMessage(msg)
    }
    updateBody() {

        let x = this.decimalTo16BitsHexString($('#body-position-x')[0].value).toString(16);
        let y = this.decimalTo16BitsHexString($('#body-position-y')[0].value).toString(16);
        let z = this.decimalTo16BitsHexString($('#body-position-z')[0].value).toString(16);
        let angle_x = this.decimalTo16BitsHexString($('#body-rotation-x')[0].value).toString(16);
        let angle_y = this.decimalTo16BitsHexString($('#body-rotation-y')[0].value).toString(16);
        let angle_z = this.decimalTo16BitsHexString($('#body-rotation-z')[0].value).toString(16);
        let msg = new Message().build(Cluters.ClusterName.BODY, Cluters.CommandBody.SET_X_Y_Z, 14, [
            x.substring(2), x.substring(0, 2),
            y.substring(2), y.substring(0, 2),
            z.substring(2), z.substring(0, 2),
            angle_x.substring(2), angle_x.substring(0, 2),
            angle_y.substring(2), angle_y.substring(0, 2),
            angle_z.substring(2), angle_z.substring(0, 2),
            0xf4, 0x01
        ]);
        this.communication.sendMessage(msg)
    }

    decimalTo8BitsHexString(number) {
        if (number < 0) {
            return parseInt(number >>> 0).toString(16).substring(6).toUpperCase();
        }
        return parseInt(number).toString(16).padStart(2, '0').toUpperCase();
    }

    decimalTo16BitsHexString(number) {
        if (number < 0) {
            return parseInt(number >>> 0).toString(16).substring(4).toUpperCase();
        }
        return parseInt(number).toString(16).padStart(4, '0').toUpperCase();
    }
}

