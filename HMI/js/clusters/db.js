export let clusters = {
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
        ALL: 0,
        ACC: {x: 0, y: 0, z: 0},
        GYR: {x: 0, y: 0, z: 0},
        MAG: {x: 0, y: 0, z: 0},
        TMP: 0
    },
    PROXIMITY: {
        US_LEFT: 0,
        US_RIGHT: 0,
        LASER: 0,
        SET_THRESHOLD: 0,
    },
    SERVO: {
        GET_ALL: 0,
        GET_ANGLE: 0,
        SET_ANGLE: 0,
        GET_MIN: 0,
        SET_MIN: 0,
        GET_MAX: 0,
        SET_MAX: 0,
        GET_OFFSET: 0,
        SET_OFFSET: 0,
        GET_STATE: 0,
        SET_STATE: 0,
        GET_REVERSE: 0,
        SET_REVERSE: 0,
        SAVE: 0,
    },
    BATTERY: {
        VOLTAGE: 0.0,
        STATUS: 0,
    },
    BODY: {
        POSITION: {x: 0, y: 0, z: 0},
        ROTATION: {x: 0, y: 0, z: 0}
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