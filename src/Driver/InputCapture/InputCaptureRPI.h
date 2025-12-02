#pragma once

#include "../Gpio/GpioInterface.h"
#include "InputCaptureInterface.h"

namespace Driver
{
    namespace InputCapture
    {
        class InputCapture : public InputCaptureInterface {
        public:
            InputCapture(Gpio::GpioInterface &gpio);

            ~InputCapture() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void EdgeChange(void) final override;

            virtual uint64_t GetInputCaptureTime(void) final override;

            virtual void ResetInputCaptureTime(void) final override;

        private:
            Gpio::GpioInterface &mGpio;
            bool mState;
            uint64_t mStartTime;
            uint64_t mDelay;
        };
    } // namespace InputCapture
} // namespace Driver