import {
    DoubleSide, Group,
    Mesh,
    MeshLambertMaterial,
    Object3D,
    SphereGeometry
} from "three";

export enum SoundSide {
    left,
    right
}

export default class SoundObject extends Object3D {
    x: number;
    y: number;
    z: number;
    soundLeft: Group;
    soundRight: Group;

    constructor() {
        super();
        this.x = 0;
        this.y = 2;
        this.z = -2.5;

        this.soundLeft = this.createSound();
        this.soundLeft.position.set(this.x - 1, this.y, this.z);
        this.soundLeft.rotation.y = Math.PI / 6;

        this.soundRight = this.createSound();
        this.soundRight.position.set(this.x + 1, this.y, this.z);
        this.soundRight.rotation.y = -Math.PI / 6;

    }

    createSound(): Group {
        let groupSound = new Group();
        let geometry = new SphereGeometry(0.2, 32, 16, Math.PI, Math.PI, Math.PI, Math.PI);
        const material = new MeshLambertMaterial({color: '#b65043'});
        material.side = DoubleSide;
        let sound = new Mesh(geometry, material);
        sound.position.set(0, 0, 0);
        groupSound.add(sound);

        geometry = new SphereGeometry(0.25, 32, 16, Math.PI, Math.PI, Math.PI, Math.PI);
        sound = new Mesh(geometry, material);
        sound.position.set(0, 0, 0.3);
        groupSound.add(sound);

        geometry = new SphereGeometry(0.3, 32, 16, Math.PI, Math.PI, Math.PI, Math.PI);
        sound = new Mesh(geometry, material);
        sound.position.set(0, 0, 0.6);

        groupSound.userData = {playing: false};
        groupSound.add(sound);

        return groupSound;
    }

    show(delay: number, side: SoundSide) {
        if (side == SoundSide.left) {
            if (this.soundLeft.userData.playing == false) {
                this.soundLeft.userData = {playing: true};
                this.displaySoundMovement(delay, this.soundLeft, SoundSide.left, delay);
            }
        } else {
            if (this.soundRight.userData.playing == false) {
                this.soundRight.userData = {playing: true};
                this.displaySoundMovement(delay, this.soundRight, SoundSide.right, delay);
            }
        }
    }

    displaySoundMovement(delay: number, sound: Group, side: SoundSide, nbIteration: number) {
        this.add(sound);
        let soundComing = setInterval(() => {
            if (side == SoundSide.left) {
                sound.position.x += 0.025;
                sound.position.z += 0.025;
            } else {
                sound.position.x -= 0.025;
                sound.position.z += 0.025;
            }

        }, 50)
        setTimeout(() => {
            clearInterval(soundComing);
            this.remove(sound);
            if (side == SoundSide.left) {
                sound.position.set(this.x - 1, this.y, this.z);
            } else {
                sound.position.set(this.x + 1, this.y, this.z);
            }
            if (nbIteration > 1) {
                nbIteration = nbIteration - 1;
                this.displaySoundMovement(delay, sound, side, nbIteration--);
            } else {
                sound.userData.playing = false;
            }
        }, 600)
    }
}