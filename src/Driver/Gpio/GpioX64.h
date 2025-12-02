#pragma once

#include "GpioInterface.h"

namespace Driver
{
    namespace Gpio
    {
        class Gpio : public GpioInterface {
        public:
            Gpio(const SGpio &gpio, const EPortDirection &portDirection);

            ~Gpio() = default;

            virtual Core::Status Set(void) final override;

            virtual Core::Status Reset(void) final override;

            virtual bool Get(void) final override;

            virtual SGpio &GetPin(void) final override;

            virtual void SetInterruptPin(void) final override;

            virtual void SetInterruptPin(const InterruptCallback callback) final override;

            virtual void ResetInterruptPin(void) final override;

        private:
            SGpio mGpio;
        };
    } // namespace Gpio
} // namespace Driver