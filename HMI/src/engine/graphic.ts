import {WebGLRenderer, Clock, Scene} from 'three'
import {getCanvas} from '../tool/function.ts'
import Camera from "./camera.ts";

export default class Graphic extends WebGLRenderer {
    scene: Scene;
    canvas: HTMLCanvasElement;
    clock: Clock = new Clock();
    camera: Camera;
    cbUpdate: FrameRequestCallback | undefined;
    cbLoop: FrameRequestCallback;

    constructor(scene: Scene, camera: Camera) {
        const canvas = getCanvas();
        super({canvas, antialias: true});
        this.canvas = canvas;
        this.scene = scene
        this.camera = camera
        this.cbLoop = this.loop.bind(this)
        this.shadowMap.enabled = true;

        window.addEventListener('resize', this.onWindowResize.bind(this));
        this.loop()
    }

    loop() {
        const dt = this.clock.getDelta()
        if (this.cbUpdate) this.cbUpdate(dt)
        this.render(this.scene, this.camera)
        requestAnimationFrame(this.cbLoop)
    }

    onUpdate(callback: FrameRequestCallback) {
        this.cbUpdate = callback
    }

    onWindowResize() {
        this.camera.resize(window.innerWidth / window.innerHeight)
        this.setSize(window.innerWidth, window.innerHeight);
    };
}