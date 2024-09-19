import * as THREE from 'three';
import Hexapod from './hexapod/hexapod.js'
import {OrbitControls} from 'three/addons/controls/OrbitControls.js';
//import Stats from 'stats.js'
import RayCasting from "./raycasting/RayCasting.js";
import Controls from "./gui/Controls.js";


export default class Canvas {

    constructor(width, height) {
        this.canvas = $("#myCanvas")[0];
        this.width = width;
        this.height = height;

        //this.stats = Stats();
        //document.body.appendChild(this.stats.dom)

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

        this.guiControls = new Controls();
        this.hexapodFolder = this.guiControls.addFolder(this.guiControls.gui, 'Hexapod');

        this.raycasting = new RayCasting(this.scene, this.camera, this.width, this.height, (memberClicked) => {
            console.log(memberClicked);
            hexapod[memberClicked] = {
                position: {
                    x: 80 / 2,
                    y: 0,
                    z: 150 / 2
                },
                rotation: {
                    x: 0,
                    y: -Math.PI / 5,
                    z: 0
                }
            };/*
            this.membersFolder = this.guiControls.addFolder(this.hexapodFolder, 'Members');
            this.memberFolder = this.guiControls.addFolder(this.membersFolder, memberClicked);
            let memberPositionFolder = this.guiControls.addFolder(this.memberFolder, 'position');
            let memberRotationFolder = this.guiControls.addFolder(this.memberFolder, 'rotation');

            let memberPositionX = memberPositionFolder.add(hexapod[memberClicked].position, 'x').min(-100).max(100);
            memberPositionFolder.add(hexapod[memberClicked].position, 'y').min(-100).max(100);
            memberPositionFolder.add(hexapod[memberClicked].position, 'z').min(-100).max(100);
            memberPositionX.onFinishChange(function (value) {
                console.log('position.' + this.property + ' changed to:', value);
            });
            memberRotationFolder.add(hexapod[memberClicked].rotation, 'x').min(-Math.PI).max(Math.PI);
            memberRotationFolder.add(hexapod[memberClicked].rotation, 'y').min(-Math.PI).max(Math.PI);
            memberRotationFolder.add(hexapod[memberClicked].rotation, 'z').min(-Math.PI).max(Math.PI);
*/
        });
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
        this.guiControls.updateDisplay();
        this.renderer.render(this.scene, this.camera);
    }


}
