#pragma once

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
    }
}
