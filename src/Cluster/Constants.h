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
        BODY = 5U,
        BUTTON = 6U,
        SOUND = 7U
    };

    static constexpr uint8_t NB_CLUSTERS = 8U;


    enum EClusterCommandGeneric { GENERIC = 0xFFU };

    enum EGeneralCommands {
        RESET = 0x00U,
        VERSION,
        MIN_EXECUTION_TIME,
        MAX_EXECUTION_TIME,
        RESET_EXECUTION_TIME,
    };

    static constexpr uint8_t NB_COMMANDS_GENERAL = 5U;

    enum EBatteryCommands {
        GET_BAT_STATUS = 0x00U,
        GET_VOLTAGE = 0x01U,
        GET_CURRENT = 0x02U,
    };

    static constexpr uint8_t NB_COMMANDS_BATTERY = 3U;

    enum EBodyCommands {
        SET_BODY_POS_ROT = 0x00U,
        SET_LEG_POS_ROT,
        SET_WALK_STATUS,
        SET_DIRECTION,
        GET_DIRECTION,
        SET_AMPLITUDE,
        GET_AMPLITUDE,
        SET_ELEVATION,
        GET_ELEVATION,
        GET_DIRECTION_AMPLITUDE_ELEVATION
    };

    static constexpr uint8_t NB_COMMANDS_BODY = 10U;

    enum EImuCommands {
        ALL,
        ACC,
        GYR,
        MAG,
        TMP,
        YAW_PITCH_ROLL,
        PRESSURE,
        ALTITUDE,
        TMP_BAR,
        CALIB_SENSOR,
    };

    static constexpr uint8_t NB_COMMANDS_IMU = 11U;

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

    static constexpr uint8_t NB_COMMANDS_SERVO = 15U;


    enum EProximityCommands {
        US_LEFT,
        US_RIGHT,
        LASER,
        SET_THRESHOLD,
    };

    static constexpr uint8_t NB_COMMANDS_PROXIMITY = 4U;


    enum EButtonCommands {
        GET_BP_STATUS = 0x00U,
    };

    static constexpr uint8_t NB_COMMANDS_BUTTON = 1U;


    enum ESoundCommands {
        GET_SOUND_STATUS = 0x00U,
    };

    static constexpr uint8_t NB_COMMANDS_SOUND = 1U;
} // namespace Cluster
