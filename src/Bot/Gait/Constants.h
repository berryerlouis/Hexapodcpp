#pragma once

#include <stdint.h>
#include <string>

namespace Bot
{
    namespace Gait
    {
        enum GaitType : uint8_t {
            TRIPOD = 0x00U,
            WAVE,
            RIPPLE,
            DOUBLE_WAVE
        };

        static constexpr uint8_t NB_GAIT_TYPES = 4U;

        static inline std::string GaitTypeToString(const GaitType gait) {
            switch (gait) {
                case TRIPOD:
                    return "TRIPOD";
                case WAVE:
                    return "WAVE";
                case RIPPLE:
                    return "RIPPLE";
                case DOUBLE_WAVE:
                    return "DOUBLE_WAVE";
                default:
                    return "UNKNOWN";
            }
        }
    };
}
