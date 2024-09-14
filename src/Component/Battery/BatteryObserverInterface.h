#pragma once

#include "BatteryState.h"

namespace Component
{
    namespace Battery
    {
        class BatteryObserverInterface {
        public:
            BatteryObserverInterface() = default;

            ~BatteryObserverInterface() = default;

            virtual void UpdatedBatteryState(const BatteryState &batteryState, const uint16_t voltage) = 0;
        };
    }
}
