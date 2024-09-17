import * as THREE from 'three';
import Servo from './servo.js'


const COXA_LENGTH = 25.4;
const FEMUR_LENGTH = 85.0;
const TIBIA_LENGTH = 127.0;

export default class Leg extends THREE.Group {

    constructor(name, leg) {
        super();

        this.name = "leg-" + name;
        this.position.x = leg.position.x;
        this.position.y = hexapod.y + leg.position.y + 5;
        this.position.z = leg.position.z;
        this.rotation.y = leg.angle.y;


        this.groupcoxa = new THREE.Group();
        this.groupcoxa.name = "Coxa";
        this.groupfemur = new THREE.Group();
        this.groupfemur.name = "Femur";
        this.groupTibia = new THREE.Group();
        this.groupTibia.name = "Tibia";

        //Coxa servo
        let coxaServo = new Servo("coxa-" + this.name,
            {
                x: 0,
                y: 0,
                z: -hexapod.body.servo.height / 4
            },
            {
                x: 0,
                y: 0,
                z: 0
            });
        let femurServo = new Servo("femur-" + this.name,
            {
                x: this.position.x > 0 ? 25.4 : -25.4,
                y: 0,
                z: -hexapod.body.servo.height / 4
            },
            {
                x: THREE.MathUtils.degToRad(-90),
                y: 0,
                z: 0
            });
        let tibiaServo = new Servo("tibia-" + this.name,
            {
                x: this.position.x > 0 ? -90 : 90,
                y: -40,
                z: -10
            },
            {
                x: 0,
                y: 0,
                z: 0
            });


        //femur
        let geometry1 = new THREE.BoxGeometry(110.4, 1.5, 20);
        let material = new THREE.MeshBasicMaterial({color: "darkgray"});
        const femur = new THREE.Mesh(geometry1, material);
        femur.position.x = femurServo.getAxe().position.x;
        femur.position.y = femurServo.getAxe().position.y - 30;
        femur.position.z = femurServo.getAxe().position.z - 30;
        femur.rotation.x = femurServo.rotation.x
        femur.rotation.y = femurServo.rotation.y;
        femur.rotation.z = femurServo.rotation.z;
        geometry1.translate(this.position.x > 0 ? -45 : 45, 5, 0)

        //tibia
        let geometry2 = new THREE.BoxGeometry(125, 1.5, 20);
        material = new THREE.MeshBasicMaterial({color: "darkgray"});
        const tibia = new THREE.Mesh(geometry2, material);
        tibia.position.x = tibiaServo.getAxe().position.x;
        tibia.position.y = tibiaServo.getAxe().position.y;
        tibia.position.z = tibiaServo.getAxe().position.z - 20;
        tibia.rotation.x = tibiaServo.rotation.x;
        tibia.rotation.y = tibiaServo.rotation.y + THREE.MathUtils.degToRad(90);
        tibia.rotation.z = tibiaServo.rotation.z;
        geometry2.translate(45, 0, 0)

        this.groupTibia.add(tibia);
        this.groupTibia.add(tibiaServo);
        femur.add(this.groupTibia);

        this.groupfemur.add(femurServo);
        this.groupfemur.add(femur);

        this.groupcoxa.add(coxaServo);
        this.groupcoxa.add(this.groupfemur);

        this.add(this.groupcoxa);
    }


    move(coxa, femur, tibia) {
        this.groupcoxa.rotation.y = THREE.MathUtils.degToRad(coxa);
        this.groupfemur.children[1].rotation.y = THREE.MathUtils.degToRad(180) + THREE.MathUtils.degToRad(femur);
        this.groupTibia.children[0].rotation.y = THREE.MathUtils.degToRad(-90) + THREE.MathUtils.degToRad(tibia);
    }


    moveServo(servoId, angle) {
        if (servoId === 0) this.moveCoxa(angle);
        if (servoId === 1) this.moveFemur(angle);
        if (servoId === 2) this.moveTibia(angle);
    }


    moveCoxa(coxa) {
        this.groupcoxa.rotation.y = -THREE.MathUtils.degToRad(coxa);
    }

    moveFemur(femur) {
        this.groupfemur.children[1].rotation.y = THREE.MathUtils.degToRad(180) + THREE.MathUtils.degToRad(femur);
    }

    moveTibia(tibia) {
        this.groupTibia.children[0].rotation.y = THREE.MathUtils.degToRad(-90) + THREE.MathUtils.degToRad(-tibia);
    }
}