import * as THREE from "three";
import {Color} from "three";

export default class RayCasting {
    constructor(scene, camera, width, height, cbMemberClicked) {
        this.width = width;
        this.height = height;
        this.camera = camera;
        this.scene = scene;
        this.raycaster = new THREE.Raycaster();
        this.selectedMesh = null;
        this.cbMemberClicked = cbMemberClicked;
        window.addEventListener('mousedown', this.onPointerMove.bind(this));
    }

    onPointerMove(event) {
        let pointer = new THREE.Vector2();
        pointer.x = (event.clientX / this.width) * 2 - 1;
        pointer.y = -((event.clientY - document.body.children[0].children[0].clientHeight) / this.height) * 2 + 1;

        this.raycaster.setFromCamera(pointer, this.camera);

        // calculate objects intersecting the picking ray
        const intersects = this.raycaster.intersectObjects(this.scene.children);

        this.selectedMesh && this.dislightPointedMember();
        this.selectedMesh = null;
        for (let i = 0; i < intersects.length; i++) {
            if (intersects[i].object.name !== "mesh" && intersects[i].object.name !== "grid" && !intersects[i].object.name.includes("group")) {
                this.selectedMesh = intersects[i].object;
                break;
            }
        }
        this.selectedMesh && this.highlightPointedMember();
    }

    highlightPointedMember() {
        if (this.selectedMesh.parent && !this.selectedMesh.parent.name.includes("group")) {
            this.selectedMesh.savedColor = [];
            for (const child of this.selectedMesh.parent.children) {
                this.selectedMesh.savedColor.push(new Color(child.material.color));
                child.material.color.set(0x9d0000);
            }
            this.cbMemberClicked && this.cbMemberClicked(this.selectedMesh.parent.name);
        } else {
            this.selectedMesh.savedColor = new Color(this.selectedMesh.material.color);
            this.selectedMesh.material.color.set(0x9d0000);
            this.cbMemberClicked && this.cbMemberClicked(this.selectedMesh.name);
        }
    }

    dislightPointedMember() {
        if (this.selectedMesh.parent && !this.selectedMesh.parent.name.includes("group")) {
            for (const child of this.selectedMesh.parent.children) {
                child.material.color.set(this.selectedMesh.savedColor.shift());
            }
        } else {
            this.selectedMesh.material.color.set(this.selectedMesh.savedColor);
        }
    }
}