import {BoxGeometry, Color, DoubleSide, Mesh, MeshLambertMaterial, Object3D, Vector3} from "three";
import Socket from "../../communication/socket";

export enum ProximitySide {
    left,
    right,
    center
}

export default class ProximityObject extends Object3D {
    x: number;
    y: number;
    z: number;
    leftWorldPoint: Mesh | null;
    rightWorldPoint: Mesh | null;
    centerWorldTrail: Mesh[][][]; // [i][j][traceIndex]
    socket: Socket;

    constructor(socket: Socket) {
        super();
        this.socket = socket;
        this.x = 0;
        this.y = 0;
        this.z = 0;

        this.leftWorldPoint = null;
        this.rightWorldPoint = null;
        this.centerWorldTrail = [];
        for (let i = 0; i < 8; i++) {
            this.centerWorldTrail[i] = [];
            for (let j = 0; j < 8; j++) {
                this.centerWorldTrail[i][j] = [];
            }
        }
        this.socket.addCallbackStarted(() => {
            this.resetTraces();
        });
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
        return new Mesh(geometry, material);
    }

    private createTrailMarker(sizeX: number, sizeY: number, sizeZ: number, distanceCm: number): Mesh {
        const mesh = this.createProximityObject(new BoxGeometry(sizeX, sizeY, sizeZ));
        mesh.userData.distanceCm = distanceCm;
        return mesh;
    }

    static MAX_TRACE_DEPTH = 100;

    private pushMarker(queue: Mesh[], marker: Mesh) {
        this.add(marker);
        queue.push(marker);
        if (queue.length > ProximityObject.MAX_TRACE_DEPTH) {
            const removed = queue.shift();
            if (removed && removed.parent) {
                removed.parent.remove(removed);
            }
        }
    }

    private applyVisualStyle(mesh: Mesh, alpha: number) {
        const blue = new Color('#4364b6');
        const red = new Color('#ff2a2a');
        const distanceCm = typeof mesh.userData.distanceCm === 'number' ? mesh.userData.distanceCm : 100;
        const t = distanceCm < 100 ? Math.max(0, Math.min(1, (100 - distanceCm) / 100)) : 0;
        const color = blue.clone().lerp(red, t);

        const material = mesh.material as MeshLambertMaterial;
        material.color.copy(color);
        material.opacity = alpha;
        material.transparent = true;
    }

    private updateQueueOpacity(queue: Mesh[]) {
        const total = queue.length;
        for (let idx = 0; idx < total; idx++) {
            const ageNorm = total > 1 ? idx / (total - 1) : 0;
            const alpha = 0.15 + ageNorm * 0.85;
            this.applyVisualStyle(queue[idx], alpha);
            queue[idx].visible = true;
        }
    }

    show(distance: number | number[][], side: ProximitySide) {
        if (side === ProximitySide.center && Array.isArray(distance)) {
            for (let i = 0; i < 8; i++) {
                for (let j = 0; j < 8; j++) {
                    const dist = distance[i][j];
                    if (dist <= 0) {
                        continue;
                    }
                    const coneAngle = Math.PI / 6; // 60° total field of view
                    const maxSpan = Math.tan(coneAngle);
                    const localX = ((i - 3.5) / 3.5) * maxSpan;
                    const localY = ((j - 3.5) / 3.5) * maxSpan;
                    const range = dist / 10;
                    const direction = new Vector3(localX, localY, -1).normalize().multiplyScalar(range);
                    const marker = this.createTrailMarker(0.08, 0.08, 0.08, dist);
                    marker.position.copy(direction);
                    this.pushMarker(this.centerWorldTrail[i][j], marker);
                    this.updateQueueOpacity(this.centerWorldTrail[i][j]);
                }
            }
        } else if (typeof distance === 'number') {
            if (distance > 0) {
                const sideSign = side === ProximitySide.left ? -1 : 1;
                const range = distance / 10;
                const sensorOriginX = sideSign * 0.1;
                const rayDirection = new Vector3(sideSign * Math.sin(Math.PI / 3), 0, -Math.cos(Math.PI / 3)).normalize().multiplyScalar(range);
                const localPoint = new Vector3(sensorOriginX, 0, 0).add(rayDirection);

                if (side === ProximitySide.left) {
                    if (!this.leftWorldPoint) {
                        this.leftWorldPoint = this.createTrailMarker(0.12, 0.12, 0.12, distance);
                        this.add(this.leftWorldPoint);
                    }
                    this.leftWorldPoint.position.copy(localPoint);
                    this.leftWorldPoint.userData.distanceCm = distance;
                    this.applyVisualStyle(this.leftWorldPoint, 1);
                } else if (side === ProximitySide.right) {
                    if (!this.rightWorldPoint) {
                        this.rightWorldPoint = this.createTrailMarker(0.12, 0.12, 0.12, distance);
                        this.add(this.rightWorldPoint);
                    }
                    this.rightWorldPoint.position.copy(localPoint);
                    this.rightWorldPoint.userData.distanceCm = distance;
                    this.applyVisualStyle(this.rightWorldPoint, 1);
                }
            } else {
                if (side === ProximitySide.left && this.leftWorldPoint && this.leftWorldPoint.parent) {
                    this.leftWorldPoint.parent.remove(this.leftWorldPoint);
                    this.leftWorldPoint = null;
                } else if (side === ProximitySide.right && this.rightWorldPoint && this.rightWorldPoint.parent) {
                    this.rightWorldPoint.parent.remove(this.rightWorldPoint);
                    this.rightWorldPoint = null;
                }
            }
        }
    }

    // Reset all traces
    resetTraces() {
        if (this.leftWorldPoint && this.leftWorldPoint.parent) {
            this.leftWorldPoint.parent.remove(this.leftWorldPoint);
        }
        if (this.rightWorldPoint && this.rightWorldPoint.parent) {
            this.rightWorldPoint.parent.remove(this.rightWorldPoint);
        }
        this.leftWorldPoint = null;
        this.rightWorldPoint = null;

        for (let i = 0; i < 8; i++) {
            for (let j = 0; j < 8; j++) {
                const queue = this.centerWorldTrail[i][j];
                for (let idx = 0; idx < queue.length; idx++) {
                    const marker = queue[idx];
                    if (marker.parent) {
                        marker.parent.remove(marker);
                    }
                }
                this.centerWorldTrail[i][j] = [];
            }
        }

    }
}