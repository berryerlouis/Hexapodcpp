import * as THREE from 'three';
import Hexapod from './Hexapod/hexapod.js'
import { OrbitControls } from 'three/addons/controls/OrbitControls.js';
import { GLTFLoader } from 'three/addons/loaders/GLTFLoader.js';


export default class Canvas {

    constructor(width, height) {
        this.canvas = $("#myCanvas")[0];

        this.scene = new THREE.Scene();
        this.camera = new THREE.PerspectiveCamera(75, width / height, 0.1, 100000);
        this.camera.position.set(0, 600, 0);

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setSize(width, height);
        this.canvas.append(this.renderer.domElement);
        this.controls = new OrbitControls(this.camera, this.renderer.domElement);
        this.controls.update();

        this.scene.background = new THREE.Color(0xe0e0e0);
        this.scene.fog = new THREE.Fog(0xe0e0e0, 2000, 20000);
        this.addGround();
        this.addLights();

        this.groupBody = new THREE.Group();

        this.robot = new Hexapod(this.groupBody);

        this.scene.add(this.groupBody);
    }

    addLights() {
        // lights
        const hemiLight = new THREE.HemisphereLight(0xffffff, 0x444444);
        hemiLight.position.set(0, 20, 0);
        this.scene.add(hemiLight);

        const dirLight = new THREE.DirectionalLight(0xffffff);
        dirLight.position.set(0, 20, 10);
        this.scene.add(dirLight);
    }

    addGround() {
        // ground
        const mesh = new THREE.Mesh(new THREE.PlaneGeometry(20000, 20000), new THREE.MeshPhongMaterial({ color: 0x999999, depthWrite: false }));
        mesh.rotation.x = - Math.PI / 2;
        this.scene.add(mesh);

        // ground grid
        const grid = new THREE.GridHelper(20000, 500, 0x000000, 0x000000);
        grid.material.opacity = 0.4;
        grid.material.transparent = true;
        this.scene.add(grid);
    }

    drawObstacleLeft(dist) {
        hexapod.head.srfLeft = dist;
        this.robot.drawObstacle();
    }
    drawObstacleCenter(dist) {
        hexapod.head.vlx = dist;
        this.robot.drawObstacle();
    }
    drawObstacleRight(dist) {
        hexapod.head.srfRight = dist;
        this.robot.drawObstacle();
    }

    moveLeg(legId, coxa, femur, tibia) {
        this.robot.moveLeg(legId, coxa, femur, tibia)
    }

    moveServo(servoId, angle) {
        this.robot.moveServo(servoId, angle)
    }

    animate() {
        this.renderer.render(this.scene, this.camera);
    }
}
