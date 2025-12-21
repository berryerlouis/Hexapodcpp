#pragma once

#include "../Gpio/GpioInterface.h"
#include "AdcInterface.h"

namespace Driver
{
    namespace Adc
    {
        class Adc : public AdcInterface {
        public:
            Adc(const Gpio::GpioInterface &gpio);

            ~Adc() {};

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         StartConversion(void) final override;

            virtual uint16_t     Read(void) final override;
        };
    } // namespace Adc
} // namespace Driver
