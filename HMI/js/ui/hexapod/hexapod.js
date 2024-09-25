import * as THREE from 'three';
import Leg from './leg.js'
import drawBody from './body.js'


export default class Hexapod {

    constructor(groupBody) {
        this.name = 'Hexapod';
        this.groupBody = groupBody;
        this.initBody();
        this.initHead();
    }

    initBody() {
        this.legs = [];
        this.groupBody.add(drawBody("bottom", 0));
        this.groupBody.add(drawBody("top", 50));
        this.legs.push(new Leg("FL", hexapod.body.legFL));
        this.legs.push(new Leg("ML", hexapod.body.legML));
        this.legs.push(new Leg("RL", hexapod.body.legRL));
        this.legs.push(new Leg("FR", hexapod.body.legFR));
        this.legs.push(new Leg("MR", hexapod.body.legMR));
        this.legs.push(new Leg("RR", hexapod.body.legRR));
        this.groupBody.add(...this.legs);
        this.rot = 0
    }

    initHead() {
        this.sensors = [];
        let geometry = new THREE.BoxGeometry(80, 50, 1);
        let material = new THREE.MeshBasicMaterial({color: 0x00ff00});
        let cylinder = new THREE.Mesh(geometry, material);
        cylinder.position.x = 0
        cylinder.position.z = -hexapod.body.height / 2 - 8
        cylinder.position.y += 25 + hexapod.y;
        cylinder.rotateY(30 * 3.14 / 180);
        cylinder.translateZ(-100 - hexapod.head.srfLeft)
        this.sensors.push(cylinder);

        cylinder = new THREE.Mesh(new THREE.BoxGeometry(10, 10, 1), new THREE.MeshBasicMaterial({color: 0xff0000}));
        cylinder.position.x = 0
        cylinder.position.z = -hexapod.body.height / 2 - 8
        cylinder.position.y += 25 + hexapod.y;
        cylinder.rotateY(0 * 3.14 / 180);
        cylinder.translateZ(-100 - hexapod.head.vlx)
        this.sensors.push(cylinder);

        cylinder = new THREE.Mesh(geometry, material);
        cylinder.position.x = 0
        cylinder.position.z = -hexapod.body.height / 2 - 8
        cylinder.position.y += 25 + hexapod.y;
        cylinder.rotateY(-30 * 3.14 / 180);
        cylinder.translateZ(-100 - hexapod.head.srfRight)
        this.sensors.push(cylinder);
        this.groupBody.add(...this.sensors);
    }

    drawObstacleLeft() {
        this.sensors[0].position.x = 0
        this.sensors[0].position.z = -50 - hexapod.body.height / 2 - 8
        if (-hexapod.head.srfLeft * 10 < -100) {
            this.sensors[0].translateZ(-hexapod.head.srfLeft * 10)
        } else {
            this.sensors[0].translateZ(-50)
        }
    }

    drawObstacleCenter() {
        this.sensors[1].position.x = 0
        this.sensors[1].position.z = -50 - hexapod.body.height / 2 - 8
        this.sensors[1].translateZ(-hexapod.head.vlx * 10)
    }

    drawObstacleRight() {
        this.sensors[2].position.x = 0
        this.sensors[2].position.z = -50 - hexapod.body.height / 2 - 8
        if (-hexapod.head.srfRight * 10 < -100) {
            this.sensors[2].translateZ(-hexapod.head.srfRight * 10)
        } else {
            this.sensors[2].translateZ(-50)
        }
    }

    moveLeg(legId, coxa, femur, tibia) {
        this.legs[legId].move(coxa, femur, tibia)
    }

    moveServo(servoId, angle) {
        this.legs[parseInt(servoId / 3)].moveServo(servoId % 3, angle)
    }
}
