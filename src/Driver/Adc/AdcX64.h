#pragma once

#include "../Gpio/GpioInterface.h"
#include "AdcInterface.h"

namespace Driver {
    namespace Adc {
        class Adc : public AdcInterface {
        public:
            Adc( Gpio::GpioInterface &gpio ) {
                (void)gpio;
            }
            ~Adc() = default;

            virtual Core::CoreStatus Initialize( void ) final override
            {
                return ( Core::CoreStatus::CORE_OK );
            }

            virtual void Update( const uint64_t currentTime ) final override
            {
                (void)currentTime;
            }

            virtual void StartConversion( void ) final override
            {
            }
            virtual uint16_t Read( void ) final override
            {
                return 2U;
            }
        private:
        };
    }
}
