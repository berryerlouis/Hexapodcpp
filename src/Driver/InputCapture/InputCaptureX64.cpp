#include "InputCaptureX64.h"
#ifdef RPI
#include "wiringPi/wiringPi.h"
#endif

namespace Driver
{
    namespace InputCapture
    {
        static InputCapture *inputCapture[2U] = {};
        static uint8_t inputCaptureIndex = 0U;

        void InterruptInputCapture(void) {
            for (size_t i = 0U; i < inputCaptureIndex; i++) { inputCapture[i]->EdgeChange(); }
        }

        InputCapture::InputCapture(Gpio::GpioInterface &gpio, Tick::TickInterface &tick) :
            mGpio(gpio), mTick(tick), mState(false), mStartTime(0UL), mDelay(0UL) {
#ifdef RPI
            inputCapture[inputCaptureIndex] = this;
            inputCaptureIndex++;
            wiringPiISR(this->mGpio.GetPin().pin, INT_EDGE_BOTH, &InterruptInputCapture);
#endif

        }

        Core::Status InputCapture::Initialize(void) { return (Core::Status::CORE_OK); }

        void InputCapture::Update(const uint64_t currentTime) { (void) currentTime; }

        uint64_t InputCapture::GetInputCaptureTime(void) { return (this->mDelay); }

        void InputCapture::EdgeChange(void) {
            const int state = this->mGpio.Get();

            if (state != this->mState && state == true) { this->mStartTime = this->mTick.GetUs(); } else if (
                state != this->mState && state == false) { this->mDelay = this->mTick.GetUs() - this->mStartTime; }
            this->mState = state;
        }
    } // namespace InputCapture
} // namespace Driver
