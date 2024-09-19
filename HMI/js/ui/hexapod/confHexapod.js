const hexapod =
    {
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
                angle: {
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
                angle: {
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
                angle: {
                    x: 0,
                    y: Math.PI / 5,
                    z: 0
                }
            },
            legRR: {
                position: {
                    x: 80 / 2,
                    y: 0,
                    z: 150 / 2
                },
                angle: {
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
                angle: {
                    x: 0,
                    y: 0,
                    z: 0
                }
            },
            legFR: {
                position: {
                    x: 80 / 2,
                    y: 0,
                    z: -150 / 2
                },
                angle: {
                    x: 0,
                    y: Math.PI / 5,
                    z: 0
                }
            },
        },
        head:
            {
                vlx: 0,
                srfLeft: 0,
                srfRight: 0,
            }
    }