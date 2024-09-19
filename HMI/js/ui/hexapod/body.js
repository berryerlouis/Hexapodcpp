import * as THREE from 'three';

export default function drawBody(name, heightOffset) {

    let groupBody = new THREE.Group();
    groupBody.name = "body-" + name;
    let cylinder;
    let geometry = new THREE.BoxGeometry(2 * hexapod.body.width / 3, hexapod.body.thickness, hexapod.body.height / 1.5);
    let material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    const bodyH = new THREE.Mesh(geometry, material);
    bodyH.position.y += heightOffset + hexapod.y;
    groupBody.add(bodyH);

    geometry = new THREE.CylinderGeometry(hexapod.body.width / 4, hexapod.body.width / 4, hexapod.body.thickness, 32);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x -= hexapod.body.width / 2
    cylinder.position.z -= hexapod.body.height / 2
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.CylinderGeometry(hexapod.body.width / 4, hexapod.body.width / 4, hexapod.body.thickness, 32);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x -= hexapod.body.widthMiddle / 2
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.CylinderGeometry(hexapod.body.width / 4, hexapod.body.width / 4, hexapod.body.thickness, 32);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x -= hexapod.body.width / 2
    cylinder.position.z += hexapod.body.height / 2
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.CylinderGeometry(hexapod.body.width / 4, hexapod.body.width / 4, hexapod.body.thickness, 32);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x = hexapod.body.width / 2
    cylinder.position.z -= hexapod.body.height / 2
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.CylinderGeometry(hexapod.body.width / 4, hexapod.body.width / 4, hexapod.body.thickness, 32);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x = hexapod.body.widthMiddle / 2
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.CylinderGeometry(hexapod.body.width / 4, hexapod.body.width / 4, hexapod.body.thickness, 32);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x = hexapod.body.width / 2
    cylinder.position.z += hexapod.body.height / 2
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.RingGeometry(hexapod.body.height / 3, hexapod.body.width / 4, 32, 32, Math.PI / 7, Math.PI);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color, side: THREE.DoubleSide});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x = hexapod.body.width / 2 + 15
    cylinder.position.z = hexapod.body.height / 2 - hexapod.body.height / 4
    cylinder.rotateX(THREE.MathUtils.degToRad(90))
    cylinder.rotateZ(THREE.MathUtils.degToRad(90))
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.RingGeometry(hexapod.body.height / 3, hexapod.body.width / 4, 32, 32, -Math.PI / 7, Math.PI);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color, side: THREE.DoubleSide});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x = hexapod.body.width / 2 + 15
    cylinder.position.z -= hexapod.body.height / 2 - hexapod.body.height / 4
    cylinder.rotateX(THREE.MathUtils.degToRad(90))
    cylinder.rotateZ(THREE.MathUtils.degToRad(90))
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.RingGeometry(hexapod.body.height / 3, hexapod.body.width / 4, 32, 32, -Math.PI / 7, -Math.PI);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color, side: THREE.DoubleSide});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x -= hexapod.body.width / 2 + 15
    cylinder.position.z = hexapod.body.height / 2 - hexapod.body.height / 4
    cylinder.rotateX(THREE.MathUtils.degToRad(90))
    cylinder.rotateZ(THREE.MathUtils.degToRad(90))
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.RingGeometry(hexapod.body.height / 3, hexapod.body.width / 4, 32, 32, Math.PI / 7, -Math.PI);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color, side: THREE.DoubleSide});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x -= hexapod.body.width / 2 + 15
    cylinder.position.z -= hexapod.body.height / 2 - hexapod.body.height / 4
    cylinder.rotateX(THREE.MathUtils.degToRad(90))
    cylinder.rotateZ(THREE.MathUtils.degToRad(90))
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.RingGeometry(hexapod.body.height / 3, hexapod.body.width / 3, 32, 32, -Math.PI / 2, Math.PI);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color, side: THREE.DoubleSide});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x -= hexapod.body.width / 2 - hexapod.body.width / 2
    cylinder.position.z -= hexapod.body.height / 2 + 15
    cylinder.rotateX(THREE.MathUtils.degToRad(90))
    cylinder.rotateZ(THREE.MathUtils.degToRad(90))
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.RingGeometry(hexapod.body.height / 3, hexapod.body.width / 3, 32, 32, Math.PI / 2, Math.PI);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color, side: THREE.DoubleSide});
    cylinder = new THREE.Mesh(geometry, material);
    cylinder.position.x -= hexapod.body.width / 2 - hexapod.body.width / 2
    cylinder.position.z += hexapod.body.height / 2 + 15
    cylinder.rotateX(THREE.MathUtils.degToRad(90))
    cylinder.rotateZ(THREE.MathUtils.degToRad(90))
    cylinder.position.y += heightOffset + hexapod.y;
    groupBody.add(cylinder);

    geometry = new THREE.BoxGeometry(hexapod.body.width / 2, hexapod.body.thickness, hexapod.body.width / 2);
    material = new THREE.MeshBasicMaterial({color: hexapod.body.color});
    cylinder = new THREE.Mesh(geometry, material);
    groupBody.add(cylinder);
    cylinder.position.x = 0
    cylinder.position.z = -hexapod.body.height / 2 - 8
    cylinder.position.y += heightOffset + hexapod.y;

    return groupBody;
}
