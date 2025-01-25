import * as THREE from 'three';
import {hexapodConfiguration as hexapod, Position, Rotation } from './confHexapod.js';

interface  ServoPosition extends Position{};
interface  ServoRotation extends Rotation{};
export default class Servo extends THREE.Group {
    servoBody: THREE.Mesh;
    axe: THREE.Mesh;

    constructor(name: string, servoPosition: ServoPosition, servoRotation: ServoRotation) {
        super();
        this.name = "servo-" + name;
        this.position.x = servoPosition.x;
        this.position.y = servoPosition.y + hexapod.body.servo.depth / 2;
        this.position.z = servoPosition.z + hexapod.body.servo.height / 2;
        this.rotation.x = servoRotation.x;
        this.rotation.y = servoRotation.y - THREE.MathUtils.degToRad(180);
        this.rotation.z = servoRotation.z;

        // Draw servo body
        let geometry = new THREE.BoxGeometry(hexapod.body.servo.width, hexapod.body.servo.height, hexapod.body.servo.depth);
        let material = new THREE.MeshBasicMaterial({ color: hexapod.body.servo.color });
        this.servoBody = new THREE.Mesh(geometry, material);
        this.servoBody.name = "Servo";

        // Draw axe
        let geometryCylinder = new THREE.CylinderGeometry(hexapod.body.servo.axe.radius, hexapod.body.servo.axe.radius, hexapod.body.servo.axe.height, 12);
        material = new THREE.MeshBasicMaterial({ color: hexapod.body.servo.axe.color });
        this.axe = new THREE.Mesh(geometryCylinder, material);
        this.axe.name = "Axe";
        this.axe.translateY(hexapod.body.servo.height / 2 - hexapod.body.servo.axe.height / 2 + 8);
        this.axe.translateZ(hexapod.body.servo.height / 4);
        this.add(this.servoBody);
        this.add(this.axe);
    }

    getAxe = (): THREE.Object3D => {
        const axe = new THREE.Object3D();
        axe.position.x = this.axe.position.x + this.position.x;
        axe.position.y = this.axe.position.y + this.position.y;
        axe.position.z = this.axe.position.z + this.position.z;
        axe.rotation.x = this.axe.rotation.x;
        axe.rotation.y = this.axe.rotation.y;
        axe.rotation.z = this.axe.rotation.z;
        return axe;
    }
}