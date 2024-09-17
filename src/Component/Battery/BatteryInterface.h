#pragma once

#include "../ComponentInterface.h"
#include "BatteryObservableInterface.h"

namespace Component
{
    namespace Battery
    {
        class BatteryInterface : public ComponentInterface, public BatteryObservableInterface {
        public:
            BatteryInterface(void) = default;

            ~BatteryInterface() = default;

            virtual Core::CoreStatus Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual BatteryState GetState() = 0;

            virtual uint16_t GetVoltage() = 0;

            virtual Core::CoreStatus Attach(BatteryObserverInterface *observer) = 0;

            virtual void Notify(const BatteryState &state, const uint16_t voltage) = 0;
        };
    }
}
