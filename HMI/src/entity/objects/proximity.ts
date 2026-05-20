import {BoxGeometry, DoubleSide, Mesh, MeshLambertMaterial, Object3D} from "three";

export enum ProximitySide {
    left,
    right,
    center
}

export default class ProximityObject extends Object3D {
    x: number;
    y: number;
    z: number;
    proximityLeft: Mesh;
    proximityCenterGrid: Mesh[][];
    proximityRight: Mesh;

    constructor() {
        super();
        this.x = 0;
        this.y = 0;
        this.z = 0;

        this.proximityLeft = this.createProximityObject( new BoxGeometry(0.2, 1, 1));
        this.proximityLeft.geometry.translate(-1, 2, 0);
        this.proximityLeft.position.set(-0.1, 0, -1);
        this.proximityLeft.rotation.set(0, -1, 0);

        this.proximityCenterGrid = [];
        for (let i = 0; i < 8; i++) {
            this.proximityCenterGrid[i] = [];
            for (let j = 0; j < 8; j++) {
                const mesh = this.createProximityObject( new BoxGeometry(0.2, 0.1, 0.1));
                mesh.geometry.translate(1, 2, 0);
                mesh.position.set((i - 3.5) * 0.5, (j - 3.5) * 0.5, -1);
                mesh.rotation.set(0, Math.PI / 2, 0);
                mesh.visible = false;
                this.proximityCenterGrid[i][j] = mesh;
                this.add(mesh);
            }
        }

        this.proximityRight = this.createProximityObject( new BoxGeometry(0.2, 1, 1));
        this.proximityRight.geometry.translate(1, 1, 0);
        this.proximityRight.position.set(0.1, 0, -1);
        this.proximityRight.rotation.set(0, 1, 0);

        this.proximityLeft.visible = false;
        this.proximityRight.visible = false;
        this.add(this.proximityLeft);
        this.add(this.proximityRight);
    }

    update() {
        /*this.proximityLeft.position.x -= 0.01;
        this.proximityLeft.position.z -= 0.01;
        console.log(this.proximityLeft.position.x);
        console.log(this.proximityLeft.position.z);*/
    }

    createProximityObject(geometry: BoxGeometry): Mesh {
        const material = new MeshLambertMaterial({color: '#4364b6'});
        material.side = DoubleSide;
        let proximity = new Mesh(geometry, material);
        proximity.userData = {interval: 0};
        return new Mesh(geometry, material);
    }

    show(distance: number | number[][], side: ProximitySide) {
        if (side === ProximitySide.center && Array.isArray(distance)) {
            // Handle 8x8 grid for center
            for (let i = 0; i < 8; i++) {
                for (let j = 0; j < 8; j++) {
                    const dist = distance[i][j];
                    const mesh = this.proximityCenterGrid[i][j];
                    mesh.visible = true;
                    mesh.position.z = -1 - dist / 10;
                    clearTimeout(mesh.userData.interval);
                    mesh.userData.interval = setTimeout(() => {
                        mesh.visible = false;
                    }, 1000);
                }
            }
        } else if (typeof distance === 'number') {
            // Handle single distance for left/right
            if (distance > 0 && distance < 30) {
                let mesh: Mesh;
                if (side === ProximitySide.left) {
                    mesh = this.proximityLeft;
                } else if (side === ProximitySide.right) {
                    mesh = this.proximityRight;
                } else {
                    return; // Center handled above
                }
                mesh.visible = true;
                clearTimeout(mesh.userData.interval);
                mesh.userData.interval = setTimeout(() => {
                    mesh.visible = false;
                }, 1000);

                switch (side) {
                    case ProximitySide.left:
                        this.proximityLeft.position.x = -0.1 - distance / 10;
                        this.proximityLeft.position.z = -1 - distance / 10;
                        break;
                    case ProximitySide.right:
                        this.proximityRight.position.x = -0.1 + distance / 10;
                        this.proximityRight.position.z = -1 - distance / 10;
                        break;
                }
            }
        }
    }
}