import {
    BoxGeometry,
    BufferGeometry,
    CanvasTexture,
    CylinderGeometry,
    Group,
    Material,
    Matrix4,
    Mesh,
    MeshBasicMaterial,
    MeshStandardMaterial,
    Object3D,
    PlaneGeometry,
    SRGBColorSpace,
    Vector3,
} from 'three';
import { mergeGeometries } from 'three/addons/utils/BufferGeometryUtils.js';

// 1 scene unit = 10 cm, matching the firmware leg lengths (COXA 2.8 / FEMUR 8.4 / TIBIA 12.7 cm).
export const LEG = {
    coxa: 0.28,
    femur: 0.84,
    tibia: 1.27,
    footRadius: 0.042,
    mountY: 1,
    topPlateZ: -0.285,
    bottomPlateZ: 0.215,
    plateThickness: 0.03,
} as const;

export const GROUND_Y = LEG.mountY - LEG.tibia - LEG.footRadius;

function makeServoLabel(): CanvasTexture {
    const canvas = document.createElement('canvas');
    canvas.width = 256;
    canvas.height = 96;
    const ctx = canvas.getContext('2d')!;
    ctx.fillStyle = '#1b3a8c';
    ctx.fillRect(0, 0, 256, 96);
    ctx.fillStyle = '#ffffff';
    ctx.font = 'bold 46px sans-serif';
    ctx.textAlign = 'center';
    ctx.fillText('HITEC', 128, 50);
    ctx.font = '24px sans-serif';
    ctx.fillText('HS-645MG', 128, 82);
    const texture = new CanvasTexture(canvas);
    texture.colorSpace = SRGBColorSpace;
    return texture;
}

function metal(color: string, roughness: number, metalness = 0.9): MeshStandardMaterial {
    return new MeshStandardMaterial({ color, metalness, roughness });
}

function matte(color: string, roughness = 0.6, metalness = 0.05): MeshStandardMaterial {
    return new MeshStandardMaterial({ color, metalness, roughness });
}

export const Mat = {
    alu: metal('#1a1c20', 0.42, 0.75),
    aluDark: metal('#2a2d33', 0.45, 0.7),
    steel: metal('#9aa0a8', 0.35),
    silver: metal('#c3c8cf', 0.22, 1),
    brass: metal('#c99a3e', 0.28),
    gold: metal('#d9b24c', 0.3),
    servo: matte('#16171a', 0.55, 0.1),
    servoCap: matte('#24262b', 0.5, 0.1),
    servoLabel: new MeshStandardMaterial({ map: makeServoLabel(), roughness: 0.55 }),
    horn: matte('#f4f3ee', 0.45),
    rubber: matte('#0c0c0e', 0.95, 0),
    black: matte('#0d0e11', 0.6, 0.1),
    nylon: matte('#ece7d6', 0.6),
    pcbGreen: matte('#1c6a43', 0.5),
    pcbBlue: matte('#1b4f93', 0.5),
    pcbPurple: matte('#47298a', 0.5),
    pcbBlack: matte('#12161c', 0.45),
    terminal: matte('#1fa04a', 0.5),
    battery: matte('#1c1f24', 0.55),
    strap: matte('#ff7a1a', 0.7),
    xt60: matte('#e8c21a', 0.5),
    wireRed: matte('#c2262b', 0.6),
    ledRed: new MeshStandardMaterial({ color: '#ff3030', emissive: '#ff1010', emissiveIntensity: 0.9, roughness: 0.3 }),
    ledGreen: new MeshStandardMaterial({ color: '#3dff9a', emissive: '#12ff7a', emissiveIntensity: 0.9, roughness: 0.3 }),
    ledCyan: new MeshStandardMaterial({ color: '#2ee6d6', emissive: '#18d6c6', emissiveIntensity: 1.1, roughness: 0.3 }),
    glass: new MeshStandardMaterial({ color: '#10141b', metalness: 0.6, roughness: 0.08 }),
};

// Merges many primitives into one mesh per material to keep the draw-call count low.
export class ModelBuilder {
    private readonly parts = new Map<Material, BufferGeometry[]>();

    add(geometry: BufferGeometry, material: Material, x = 0, y = 0, z = 0): this {
        geometry.translate(x, y, z);
        const flat = geometry.index ? geometry.toNonIndexed() : geometry;
        const list = this.parts.get(material) ?? [];
        list.push(flat);
        this.parts.set(material, list);
        return this;
    }

    box(w: number, h: number, d: number, material: Material, x = 0, y = 0, z = 0): this {
        return this.add(new BoxGeometry(w, h, d), material, x, y, z);
    }

    cyl(
        radius: number,
        length: number,
        material: Material,
        x = 0,
        y = 0,
        z = 0,
        axis: 'x' | 'y' | 'z' = 'y',
        segments = 20,
        bottomRadius = radius,
    ): this {
        const geometry = new CylinderGeometry(radius, bottomRadius, length, segments);
        if (axis === 'x') {
            geometry.rotateZ(Math.PI / 2);
        } else if (axis === 'z') {
            geometry.rotateX(Math.PI / 2);
        }
        return this.add(geometry, material, x, y, z);
    }

    plane(w: number, h: number, material: Material, x: number, y: number, z: number, flip = false): this {
        const geometry = new PlaneGeometry(w, h);
        if (flip) {
            geometry.rotateY(Math.PI);
        }
        return this.add(geometry, material, x, y, z);
    }

    build(parent?: Object3D): Group {
        const group = new Group();
        for (const [material, geometries] of this.parts) {
            const merged = mergeGeometries(geometries, false);
            if (merged) {
                group.add(new Mesh(merged, material));
            }
        }
        parent?.add(group);
        return group;
    }
}

// Maps the object's local X/Y/Z axes onto the given parent-frame directions.
export function orient(obj: Object3D, xAxis: Vector3, yAxis: Vector3, zAxis: Vector3) {
    obj.quaternion.setFromRotationMatrix(new Matrix4().makeBasis(xAxis, yAxis, zAxis));
}

export function enableShadows(root: Object3D) {
    root.traverse((o) => {
        if (o instanceof Mesh && !(o.material instanceof MeshBasicMaterial)) {
            o.castShadow = true;
            o.receiveShadow = true;
        }
    });
}
