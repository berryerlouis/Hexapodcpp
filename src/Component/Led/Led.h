#pragma once

#include "LedInterface.h"
#include "../../Driver/Gpio/GpioInterface.h"

namespace Component
{
    namespace Led
    {
        class Led : public LedInterface {
        public:
            Led(Driver::Gpio::GpioInterface &gpio);

            ~Led() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual Core::CoreStatus On() final override;

            virtual Core::CoreStatus Off() final override;

            virtual Core::CoreStatus Toggle() final override;

            virtual LedState Get() const final override;

        private:
            Driver::Gpio::GpioInterface &mGpio;
            LedState mState;
        };
    }
}
