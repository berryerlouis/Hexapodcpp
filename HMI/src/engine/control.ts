import {OrbitControls} from 'three/addons/controls/OrbitControls.js';
import Camera from "./camera.ts";
import Graphic from "./graphic.ts";

export default class Control extends OrbitControls {
    constructor(camera: Camera, graphic: Graphic) {
        super(camera, graphic.domElement)

        this.minDistance = 3;
        this.maxDistance = 50;

        this.maxPolarAngle = Math.PI / 2;
    }
}