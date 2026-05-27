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
    private lastFrameTime: number = 0;
    private static readonly TARGET_FPS = 30;
    private static readonly FRAME_INTERVAL_MS = 1000 / Graphic.TARGET_FPS;

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
        requestAnimationFrame(this.cbLoop);
        const now = performance.now();
        if (now - this.lastFrameTime < Graphic.FRAME_INTERVAL_MS) return;
        this.lastFrameTime = now;
        const dt = this.clock.getDelta();
        if (this.cbUpdate) this.cbUpdate(dt);
        this.render(this.scene, this.camera);
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