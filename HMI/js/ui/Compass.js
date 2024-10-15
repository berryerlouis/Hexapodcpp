import * as THREE from "three";
import {OrbitControls} from "three/addons/controls/OrbitControls.js";

export default class Compass {

    constructor(width = 100, height = 100) {
        this.canvas = $("#myCompass")[0];
        this.width = width;
        this.height = height;
        this.scene = new THREE.Scene();
        this.scene.name = 'scene';
        this.camera = new THREE.PerspectiveCamera(75, this.width / this.height, 0.1, 1000);
        this.camera.name = 'camera';
        this.camera.position.set(0, 100, 0);

        this.renderer = new THREE.WebGLRenderer();
        this.renderer.setSize(this.width, this.height);
        this.canvas.append(this.renderer.domElement);
        this.controls = new OrbitControls(this.camera, this.renderer.domElement);
        this.controls.enableRotate = false
        //this.controls.update();

        this.scene.background = new THREE.Color(0x505050);
        this.addLights();

        const geometr = new THREE.SphereGeometry(60, 32, 16);
        const materia = new THREE.MeshBasicMaterial({color: 0x00ff00});
        this.spere = new THREE.Mesh(geometr, materia);
        this.scene.add(this.spere);

        const geometry = new THREE.SphereGeometry(60, 32, 16, -Math.PI / 2 - Math.PI / 16, Math.PI / 8, 0, Math.PI);
        const material = new THREE.MeshBasicMaterial({color: 0xff0000});
        this.north = new THREE.Mesh(geometry, material);
        this.scene.add(this.north);
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

    rotate(angle) {
        this.north.rotation.y = -(angle / 180) * Math.PI;
    }

    animate() {
        this.renderer.render(this.scene, this.camera);
    }
}