import { PerspectiveCamera } from 'three'

export default class Camera extends PerspectiveCamera {
    constructor() {
        super(70, innerWidth / innerHeight)
        this.position.set(0, 5.4, 5)
        this.lookAt(0, 0, 1.8)
    }
    resize(aspectRation: number) {
        this.aspect = aspectRation;
        this.updateProjectionMatrix();
    }
    update() {
    }
}