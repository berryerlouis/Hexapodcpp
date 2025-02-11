#include "InputCapture.h"

namespace Driver
{
    namespace InputCapture
    {
        InputCapture::InputCapture(Gpio::GpioInterface &gpio, Tick::TickInterface &tick) :
            mGpio(gpio), mTick(tick), mState(false), mStartTime(0UL), mDelay(0UL) {
        }

        Core::Status InputCapture::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void InputCapture::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        uint64_t InputCapture::GetInputCaptureTime(void) {
            return (this->mDelay);
        }

        void InputCapture::ResetInputCaptureTime(void) {
            this->mDelay = 0U;
        }

        void InputCapture::EdgeChange(void) {
            const int state = this->mGpio.Get();

            if (state != this->mState && state == true) {
                this->mStartTime = this->mTick.GetUs();
            } else if (
                state != this->mState && state == false) {
                this->mDelay = this->mTick.GetUs() - this->mStartTime;
            }
            this->mState = state;
        }
    } // namespace InputCapture
} // namespace Driver
