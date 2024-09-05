#pragma once

#include "BatteryObserverInterface.h"

namespace Component
{
    namespace Battery
    {
        class BatteryObservableInterface {
        public:
            BatteryObservableInterface() = default;

            ~BatteryObservableInterface() = default;

            virtual Core::CoreStatus Attach(BatteryObserverInterface *observer) = 0;

            virtual void Notify(const BatteryState &object) = 0;
        };
    }
}
