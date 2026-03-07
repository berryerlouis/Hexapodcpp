#include "InputCapture.h"

namespace Driver
{
    namespace InputCapture
    {
        InputCapture::InputCapture(Gpio::GpioInterface &gpio)
            : mGpio(gpio)
            , mState(false)
            , mStartTime(0UL)
            , mDelay(0UL) {
        }

        Core::Status InputCapture::Initialize() {
            LOG_DRIVER_DEBUG("InputCapture", "pin %d Initialized.", this->mGpio.GetPin().pin);
            return Core::Status::CORE_OK;
        }

        void InputCapture::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        uint64_t InputCapture::GetInputCaptureTime() {
            return this->mDelay;
        }

        void InputCapture::ResetInputCaptureTime() {
            this->mDelay = 0U;
        }

        void InputCapture::EdgeChange() {
            const bool state = this->mGpio.Get();

            if (state != this->mState && state) {
                this->mStartTime = Driver::Timer::Tick::GetInstance().GetUs();
            } else if (state != this->mState && !state) {
                this->mDelay = Driver::Timer::Tick::GetInstance().GetUs() - this->mStartTime;
            }
            this->mState = state;
        }
    } // namespace InputCapture
} // namespace Driver
