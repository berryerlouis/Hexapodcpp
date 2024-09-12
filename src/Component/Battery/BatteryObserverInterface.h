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

            virtual void UpdatedBatteryState(const BatteryState &batteryState) = 0;
        };
    }
}
