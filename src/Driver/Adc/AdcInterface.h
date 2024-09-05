#pragma once

#include "../DriverInterface.h"

namespace Driver
{
    namespace Adc
    {
        class AdcInterface : public DriverInterface {
        public:
            AdcInterface() = default;

            ~AdcInterface() = default;

            virtual Core::CoreStatus Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual void StartConversion(void) = 0;

            virtual uint16_t Read(void) = 0;
        };
    }
}
