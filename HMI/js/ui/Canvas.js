import * as THREE from 'three';
import Hexapod from '../hexapod/hexapod.js'
import {OrbitControls} from 'three/addons/controls/OrbitControls.js';


export default class Canvas {

    constructor(width, height) {
        this.canvas = $("#myCanvas")[0];
        this.width = width;
        this.height = height;

        this.scene = new THREE.Scene();
        this.scene.name = 'scene';
        this.camera = new THREE.PerspectiveCamera(75, this.width / this.height, 0.1, 10000);
        this.camera.name = 'camera';
        this.camera.position.set(0, 600, 300);

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setSize(this.width, this.height);
        this.canvas.append(this.renderer.domElement);
        this.controls = new OrbitControls(this.camera, this.renderer.domElement);
        this.controls.update();

        this.scene.background = new THREE.Color(0xe0e0e0);
        this.scene.fog = new THREE.Fog(0xe0e0e0, 1500, 2000);
        this.addLights();

        this.groupBody = new THREE.Group();

        this.robot = new Hexapod(this.groupBody);
        this.robot.name = 'robot';

        this.scene.add(this.groupBody);
        this.addGround();
        this.raycaster = new THREE.Raycaster();
        this.pointer = null;

        window.addEventListener('mousemove', this.onPointerMove.bind(this));
    }

    onPointerMove(event) {
        let pointer = new THREE.Vector2();
        pointer.x = (event.clientX / this.width) * 2 - 1;
        pointer.y = -((event.clientY - document.body.children[0].children[0].clientHeight) / this.height) * 2 + 1;

        this.raycaster.setFromCamera(pointer, this.camera);

        // calculate objects intersecting the picking ray
        const intersects = this.raycaster.intersectObjects(this.scene.children);

        for (let i = 0; i < intersects.length; i++) {
            if (intersects[i].object.name !== "mesh") {
                let name = intersects[i].object.name;
                if (intersects[i].object.parent) {
                    name = intersects[i].object.parent.name;
                    for (const child of intersects[i].object.parent.children) {
                        child.material.color.set('darkgray');
                    }
                } else {
                    intersects[i].object.material.color.set(0xff0000);
                }
                console.log(name)
            }

        }

    }

    addLights() {
        // lights
        const hemiLight = new THREE.HemisphereLight(0xffffff, 0x444444);
        hemiLight.position.set(0, 20, 0);
        hemiLight.name = 'hemiLight';
        this.scene.add(hemiLight);

        const dirLight = new THREE.DirectionalLight(0xffffff);
        dirLight.position.set(0, 20, 10);
        dirLight.name = 'dirLight';
        this.scene.add(dirLight);
    }

    addGround() {
        // ground
        let mesh = new THREE.Mesh(new THREE.PlaneGeometry(20000, 20000), new THREE.MeshPhongMaterial({
            color: 0x999999,
            depthWrite: false
        }));
        mesh.rotation.x = -Math.PI / 2;
        mesh.name = 'mesh';
        this.scene.add(mesh);

        // ground grid
        let grid = new THREE.GridHelper(20000, 500, 0x000000, 0x000000);
        grid.material.opacity = 0.4;
        grid.material.transparent = true;
        grid.name = 'grid';
        this.scene.add(grid);
    }

    drawObstacleLeft(dist) {
        hexapod.head.srfLeft = dist;
        this.robot.drawObstacleLeft();
    }

    drawObstacleCenter(dist) {
        hexapod.head.vlx = dist;
        this.robot.drawObstacleCenter();
    }

    drawObstacleRight(dist) {
        hexapod.head.srfRight = dist;
        this.robot.drawObstacleRight();
    }

    moveLeg(legId, coxa, femur, tibia) {
        this.robot.moveLeg(legId, coxa, femur, tibia)
    }

    moveServo(servoId, angle) {
        this.robot.moveServo(servoId, angle)
    }

    animate() {
        if (this.pointer) {
        }

        this.renderer.render(this.scene, this.camera);
    }


}
