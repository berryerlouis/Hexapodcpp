#pragma once

#include "../ComponentInterface.h"
#include "BatteryState.h"

namespace Component
{
    namespace Battery
    {
        class BatteryInterface : public ComponentInterface<5U, BatteryStruct> {
        public:
            BatteryInterface(void) = default;

            ~BatteryInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void         Update(const uint64_t currentTime) = 0;

            virtual BatteryState GetState() = 0;

            virtual uint16_t     GetVoltage() = 0;

            virtual uint16_t     GetIntensity() = 0;
        };
    } // namespace Battery
} // namespace Component
