#pragma once

#include <stdint.h>

namespace Cluster
{
    enum EClusters {
        GENERAL = 0U,
        IMU = 1U,
        PROXIMITY = 2U,
        SERVO = 3U,
        BATTERY = 4U,
        BODY = 5U
    };

    static const uint8_t NB_CLUSTERS = 6U;

    enum EClusterCommandGeneric {
        GENERIC = 0xFFU
    };

    enum EGeneralCommands {
        VERSION,
        MIN_EXECUTION_TIME,
        MAX_EXECUTION_TIME,
        INSTANT_EXECUTION_TIME,
        RESET_EXECUTION_TIME
    };

#define NB_COMMANDS_GENERAL 5U

    enum EBatteryCommands {
        GET_VOLTAGE = 0x00U,
        GET_BAT_STATUS,
    };

#define NB_COMMANDS_BATTERY 2U

    enum EBodyCommands {
        SET_LEG_X_Y_Z = 0,
    };

#define NB_COMMANDS_BODY 1U

    enum EImuCommands {
        ALL,
        ACC,
        GYR,
        MAG,
        TMP
    };

#define NB_COMMANDS_IMU 5U

    enum EServoCommands {
        GET_ALL = 0,
        GET_ANGLE,
        SET_ANGLE,
        GET_MIN,
        SET_MIN,
        GET_MAX,
        SET_MAX,
        GET_OFFSET,
        SET_OFFSET,
        GET_STATE,
        SET_STATE,
        GET_REVERSE,
        SET_REVERSE,
        GET_STATE_PCA,
        SET_STATE_PCA,
    };

#define NB_COMMANDS_SERVO 15U

    enum EProximityCommands {
        US_LEFT,
        US_RIGHT,
        LASER,
        SET_THRESHOLD,
    };

#define NB_COMMANDS_PROXIMITY 4U
}
