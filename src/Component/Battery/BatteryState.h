#pragma once
#include <stdint.h>

namespace Component
{
    namespace Battery
    {
        enum BatteryState {
            NOMINAL = 0x00U,
            WARNING,
            CRITICAL,
            UNKNOWN = 0xFFU
        };

        struct BatteryStruct {
            const BatteryState &state;
            const uint16_t      voltage;
            const uint16_t      intensity;
        };
    } // namespace Battery
} // namespace Component
