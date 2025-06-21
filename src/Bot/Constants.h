#pragma once

#include <stdint.h>
#include <string>
#include "../Core/Logger.h"

namespace Bot
{
#define LOG_BOT_DEBUG(cmp, ...) LOG_GENERIC(DEBUG, BOT, "[" cmp "] " __VA_ARGS__)
#define LOG_BOT_INFO(cmp, ...)  LOG_GENERIC(INFO, BOT,  "[" cmp "] "  __VA_ARGS__)
#define LOG_BOT_WARNING(cmp, ...) LOG_GENERIC(WARNING, BOT, "[" cmp "] "  __VA_ARGS__)
#define LOG_BOT_ERROR(cmp, ...) LOG_GENERIC(ERROR, BOT, "[" cmp "] "  __VA_ARGS__)

    enum ELeg {
        FRONT_LEFT = 0x00,
        MIDDLE_LEFT,
        REAR_LEFT,
        FRONT_RIGHT,
        MIDDLE_RIGHT,
        REAR_RIGHT
    };

    static inline std::string ElegToString(const ELeg leg) {
        switch (leg) {
            case FRONT_LEFT:
                return "FRONT_LEFT";
            case MIDDLE_LEFT:
                return "MIDDLE_LEFT";
            case REAR_LEFT:
                return "REAR_LEFT";
            case FRONT_RIGHT:
                return "FRONT_RIGHT";
            case MIDDLE_RIGHT:
                return "MIDDLE_RIGHT";
            case REAR_RIGHT:
                return "REAR_RIGHT";
            default:
                return "UNKNOWN";
        }
    }

    static constexpr uint8_t NB_LEGS = 6U;

    enum EWalkStatus {
        PLAY = 0x00U,
        PAUSE,
        STOP,
    };

    static inline std::string WalkStatusToString(const EWalkStatus status) {
        switch (status) {
            case PLAY:
                return "PLAY";
            case PAUSE:
                return "PAUSE";
            case STOP:
                return "STOP";
            default:
                return "UNKNOWN";
        }
    }
}
