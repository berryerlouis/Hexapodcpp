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

            virtual void     StartConversion(void) = 0;

            virtual uint16_t Read(void) = 0;
        };
    } // namespace Adc
} // namespace Driver
