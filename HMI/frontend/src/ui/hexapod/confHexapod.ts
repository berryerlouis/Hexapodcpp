export interface Position {
    x: number;
    y: number;
    z: number;
}

export interface Rotation {
    x: number;
    y: number;
    z: number;
}

export interface Servo {
    axe: {
        color: string;
        radius: number;
        height: number;
    };
    color: number;
    width: number;
    height: number;
    depth: number;
}

export interface LegConfig {
    position: Position;
    rotation: Rotation;
}

export interface Body {
    width: number;
    widthMiddle: number;
    height: number;
    thickness: number;
    color: number;
    servo: Servo;
    leg: {
        color: number;
    };
    legFL: LegConfig;
    legML: LegConfig;
    legRL: LegConfig;
    legFR: LegConfig;
    legMR: LegConfig;
    legRR: LegConfig;
}

export interface Head {
    vlx: number;
    srfLeft: number;
    srfRight: number;
}

export interface Hexapod {
    x: number;
    y: number;
    z: number;
    body: Body;
    head: Head;
}

export const hexapodConfiguration: Hexapod = {
    x: 0,
    y: 90,
    z: 0,
    body: {
        width: 80,
        widthMiddle: 130,
        height: 150,
        thickness: 1.5,
        color: 0x151515,
        servo: {
            axe: {
                color: "gray",
                radius: 5,
                height: 12
            },
            color: 0x252525,
            width: 20,
            height: 40,
            depth: 40,
        },
        leg: {
            color: 0xF0F000,
        },
        legFL: {
            position: {
                x: -80 / 2,
                y: 0,
                z: -150 / 2
            },
            rotation: {
                x: 0,
                y: -Math.PI / 5,
                z: 0
            },
        },
        legML: {
            position: {
                x: -130 / 2,
                y: 0,
                z: 0
            },
            rotation: {
                x: 0,
                y: 0,
                z: 0
            }
        },
        legRL: {
            position: {
                x: -80 / 2,
                y: 0,
                z: 150 / 2
            },
            rotation: {
                x: 0,
                y: Math.PI / 5,
                z: 0
            }
        },
        legFR: {
            position: {
                x: 80 / 2,
                y: 0,
                z: 150 / 2
            },
            rotation: {
                x: 0,
                y: -Math.PI / 5,
                z: 0
            }
        },
        legMR: {
            position: {
                x: 130 / 2,
                y: 0,
                z: 0
            },
            rotation: {
                x: 0,
                y: 0,
                z: 0
            }
        },
        legRR: {
            position: {
                x: 80 / 2,
                y: 0,
                z: -150 / 2
            },
            rotation: {
                x: 0,
                y: Math.PI / 5,
                z: 0
            }
        }
    },
    head: {
        vlx: 0,
        srfLeft: 0,
        srfRight: 0,
    }
};
