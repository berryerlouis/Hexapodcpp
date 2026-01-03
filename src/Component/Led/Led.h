#pragma once

#include "../../Driver/Gpio/GpioInterface.h"
#include "LedInterface.h"

namespace Component
{
    namespace Led
    {
        class Led : public LedInterface {
        public:
            explicit Led(Driver::Gpio::GpioInterface &gpio);

            ~Led() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual Core::Status On() final override;

            virtual Core::Status Off() final override;

            virtual Core::Status Toggle() final override;

            virtual Core::Status Pwm(const uint16_t delay) final override;

            virtual LedState     Get() const final override;

        private:
            Driver::Gpio::GpioInterface &mGpio;
            LedState                     mState;
        };
    } // namespace Led
} // namespace Component
