#pragma once

#include <cstdint>

namespace Move
{
    namespace Walk
    {
#define LOG_WALK_DEBUG(cmp, ...) LOG_GENERIC(DEBUG, WALK, "[" cmp "] " __VA_ARGS__)
#define LOG_WALK_INFO(cmp, ...) LOG_GENERIC(INFO, WALK, "[" cmp "] " __VA_ARGS__)
#define LOG_WALK_WARNING(cmp, ...) LOG_GENERIC(WARNING, WALK, "[" cmp "] " __VA_ARGS__)
#define LOG_WALK_ERROR(cmp, ...) LOG_GENERIC(ERROR, WALK, "[" cmp "] " __VA_ARGS__)

        enum class EWalkStatus : uint8_t {
            WALKING = 0x00U,
            PAUSED,
            STOPPED,
            NB_WALK_STATUS
        };

        static inline const char *WalkStatusToString(const EWalkStatus status) {
            switch (status) {
                case EWalkStatus::STOPPED: return "STOPPED";
                case EWalkStatus::WALKING: return "WALKING";
                case EWalkStatus::PAUSED: return "PAUSED";
                default: return "UNKNOWN";
            }
        }
    } // namespace Walk
} // namespace Move
