import * as THREE from "three";
import { Color } from "three";

export default class RayCasting {
    private width: number;
    private height: number;
    private camera: THREE.Camera;
    private scene: THREE.Scene;
    private raycaster: THREE.Raycaster;
    private selectedMesh: any;
    private cbMemberClicked: ((name: string) => void) | null;

    constructor(scene: THREE.Scene, camera: THREE.Camera, width: number, height: number, cbMemberClicked: ((name: string) => void) | null) {
        this.width = width;
        this.height = height;
        this.camera = camera;
        this.scene = scene;
        this.raycaster = new THREE.Raycaster();
        this.selectedMesh = null;
        this.cbMemberClicked = cbMemberClicked;
        this.onPointerMove = this.onPointerMove.bind(this);
        window.addEventListener('mousedown', this.onPointerMove);
    }

    dispose() {
        window.removeEventListener('mousedown', this.onPointerMove);
    }

    private onPointerMove(event: MouseEvent) {
        const pointer = new THREE.Vector2();
        pointer.x = (event.clientX / this.width) * 2 - 1;
        pointer.y = -((event.clientY - document.body.children[0].children[0].clientHeight) / this.height) * 2 + 1;

        this.raycaster.setFromCamera(pointer, this.camera);

        const intersects = this.raycaster.intersectObjects(this.scene.children);

        if (this.selectedMesh) {
            this.dislightPointedMember();
        }
        this.selectedMesh = null;
        for (const intersect of intersects) {
            if (intersect.object.name !== "mesh" && intersect.object.name !== "grid" && !intersect.object.name.includes("group")) {
                this.selectedMesh = intersect.object;
                break;
            }
        }
        if (this.selectedMesh) {
            this.highlightPointedMember();
        }
    }

    private highlightPointedMember() {
        if (this.selectedMesh) {
            if (this.selectedMesh.parent && !this.selectedMesh.parent.name.includes("group")) {
                this.selectedMesh.savedColor = [];
                for (const child of this.selectedMesh.parent.children) {
                    this.selectedMesh.savedColor.push(new Color(child.material.color));
                    child.material.color.set(0x9d0000);
                }
                if (this.cbMemberClicked) {
                    this.cbMemberClicked(this.selectedMesh.parent.name);
                }
            } else {
                this.selectedMesh.savedColor = new Color(this.selectedMesh.material.color);
                this.selectedMesh.material.color.set(0x9d0000);
                if (this.cbMemberClicked) {
                    this.cbMemberClicked(this.selectedMesh.name);
                }
            }
        }
    }

    private dislightPointedMember() {
        if (this.selectedMesh) {
            if (this.selectedMesh.parent && !this.selectedMesh.parent.name.includes("group")) {
                for (const child of this.selectedMesh.parent.children) {
                    child.material.color.set(this.selectedMesh.savedColor.shift());
                }
            } else {
                this.selectedMesh.material.color.set(this.selectedMesh.savedColor);
            }
        }
    }
}