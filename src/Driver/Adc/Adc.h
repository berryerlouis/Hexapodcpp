#pragma once

#include "../Gpio/GpioInterface.h"
#include "AdcInterface.h"

namespace Driver
{
    namespace Adc
    {
        class Adc : public AdcInterface {
        public:
#define BRIDGE_DIVIDER    (float) ( 0.2F )
#define ADC_STEP          (float) ( 5.0 / 1024 )
#define ADC_VOLT( ADC_VALUE )    (float) ( ADC_STEP * ADC_VALUE )

            Adc(Gpio::GpioInterface &gpio);

            ~Adc() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void StartConversion(void) final override;

            virtual uint16_t Read(void) final override;

            static volatile uint16_t sAdcValue;

        private:
            Gpio::GpioInterface &mGpio;
        };
    }
}
