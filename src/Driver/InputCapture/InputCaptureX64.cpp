#include "InputCapture.h"

namespace Driver
{
    namespace InputCapture
    {
        InputCapture::InputCapture(Gpio::GpioInterface &gpio, Tick::TickInterface &tick)
            : mGpio(gpio)
              , mTick(tick)
              , mState(false)
              , mStartTime(0UL)
              , mDelay(0UL) {
        }

        Core::CoreStatus InputCapture::Initialize(void) {
            return (Core::CoreStatus::CORE_OK);
        }

        void InputCapture::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        uint64_t InputCapture::GetInputCaptureTime(void) {
            return (this->mDelay);
        }

        void InputCapture::EdgeChange(void) {
        }
    }
}
