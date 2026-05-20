import { WebGLRenderer, Clock, Scene } from 'three'
import { getCanvas } from '../tool/function.ts'
import Camera from "./camera.ts";

type UpdateCallback = (dt: number) => void;

export default class Graphic extends WebGLRenderer {
    scene: Scene;
    canvas: HTMLCanvasElement;
    clock: Clock = new Clock();
    camera: Camera;
    cbUpdate: UpdateCallback | undefined;
    cbLoop: FrameRequestCallback;
    cbResize: () => void;
    width: number;
    height: number;

    constructor(scene: Scene, camera: Camera) {
        const canvas = getCanvas();
        super({ canvas, antialias: true });
        this.canvas = canvas;
        this.scene = scene;
        this.camera = camera;
        this.cbLoop = this.loop.bind(this);
        this.cbResize = this.onWindowResize.bind(this);
        this.width = 0;
        this.height = 0;
        this.shadowMap.enabled = true;
        this.setPixelRatio(Math.min(window.devicePixelRatio || 1, 1.5));

        window.addEventListener('resize', this.cbResize);
        this.onWindowResize(); // Initial resize
        this.loop();
    }

    loop() {
        const dt = this.clock.getDelta();
        if (this.cbUpdate) this.cbUpdate(dt);
        this.render(this.scene, this.camera);
        requestAnimationFrame(this.cbLoop);
    }

    onUpdate(callback: UpdateCallback) {
        this.cbUpdate = callback;
    }

    onWindowResize() {
        const width = window.innerWidth;
        const height = window.innerHeight;

        if (width === this.width && height === this.height) {
            return;
        }

        this.width = width;
        this.height = height;
        this.camera.resize(width / height);
        this.setSize(width, height, false);
    }
}