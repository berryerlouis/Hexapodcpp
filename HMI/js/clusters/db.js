export let clustersDatabase = {
    GENERAL: {
        VERSION: "",
        MIN_EXECUTION_TIME: {
            BATTERY: 0,
            CONTROL: 0,
            COMMUNICATION: 0,
            DISPLAY: 0,
            GENERAL: 0,
            ORIENTATION: 0,
            PROXIMITY: 0
        },
        MAX_EXECUTION_TIME: {
            BATTERY: 0,
            CONTROL: 0,
            COMMUNICATION: 0,
            DISPLAY: 0,
            GENERAL: 0,
            ORIENTATION: 0,
            PROXIMITY: 0
        },
        INSTANT_EXECUTION_TIME: 0,
        RESET_EXECUTION_TIME: function () {
            alert('reset!');
        }
    },
    IMU: {
        ACC: {x: 0, y: 0, z: 0},
        GYR: {x: 0, y: 0, z: 0},
        MAG: {x: 0, y: 0, z: 0},
        TMP: 0,
        ROULIS: 0,
    },
    PROXIMITY: {
        US_LEFT: 0,
        US_RIGHT: 0,
        LASER: 0,
        SET_THRESHOLD: 0,
    },
    SERVO: {
        0: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        1: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        2: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        3: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        4: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        5: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        6: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        7: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        8: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        9: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        10: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        11: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        12: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        13: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        14: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        15: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        16: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
        17: {angle: 0, min: 0, max: 180, offset: 0, reverse: false, state: false},
    },
    BATTERY: {
        VOLTAGE: 0.0,
        STATUS: 255,
    },
    BODY: {
        POSITION: {x: 0, y: 0, z: 0},
        ROTATION: {x: 0, y: 0, z: 0},
        DELAY: 1000,
    }
};


export let services = [
    'BATTERY',
    'CONTROL',
    'COMMUNICATION',
    'DISPLAY',
    'GENERAL',
    'ORIENTATION',
    'PROXIMITY'
];
