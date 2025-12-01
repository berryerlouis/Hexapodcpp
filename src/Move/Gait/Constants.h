#pragma once

#include <stdint.h>
#include <string>
#include "../../Core/Logger.h"

namespace Move
{
    namespace Gait
    {
        
#define LOG_GAIT_DEBUG(cmp, ...) LOG_GENERIC(DEBUG, GAIT, "[" cmp "] " __VA_ARGS__)
#define LOG_GAIT_INFO(cmp, ...)  LOG_GENERIC(INFO, GAIT,  "[" cmp "] "  __VA_ARGS__)
#define LOG_GAIT_WARNING(cmp, ...) LOG_GENERIC(WARNING, GAIT, "[" cmp "] "  __VA_ARGS__)
#define LOG_GAIT_ERROR(cmp, ...) LOG_GENERIC(ERROR, GAIT, "[" cmp "] "  __VA_ARGS__)

        enum class GaitType : uint8_t {
            TRIPOD = 0,
            WAVE,
            RIPPLE,
            NB_GAITS
        };

        static inline const char* GaitTypeToString(GaitType type) {
            switch (type) {
                case GaitType::TRIPOD: return "TRIPOD";
                case GaitType::WAVE: return "WAVE";
                case GaitType::RIPPLE: return "RIPPLE";
                default: return "UNKNOWN";
            }
        }
    }
}
