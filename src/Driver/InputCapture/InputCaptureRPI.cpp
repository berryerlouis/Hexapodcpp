#include "InputCapture.h"
#include "wiringPi/wiringPi.h"

namespace Driver
{
    namespace InputCapture
    {
        static InputCapture *inputCapture[3U] = {};
        static uint8_t inputCaptureIndex = 0U;

        void InterruptInputCapture(void) {
            for (size_t i = 0U; i < inputCaptureIndex; i++) {
                inputCapture[i]->EdgeChange();
            }
        }

        InputCapture::InputCapture(Gpio::GpioInterface &gpio) :
                                                              mGpio(gpio)
                                                              , mState(false)
                                                              , mStartTime(0UL)
                                                              , mDelay(0UL) {
            inputCapture[inputCaptureIndex] = this;
            inputCaptureIndex++;
            wiringPiISR(this->mGpio.GetPin().pin, INT_EDGE_BOTH, &InterruptInputCapture);
        }

        Core::Status InputCapture::Initialize(void) {
            LOG_DRIVER_DEBUG("InputCapture", "pin %d Initialized.", this->mGpio.GetPin().pin);
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
                this->mStartTime = Tick::Tick::GetInstance().GetUs();
            } else if (
                state != this->mState && state == false) {
                this->mDelay = Tick::Tick::GetInstance().GetUs() - this->mStartTime;
            }
            this->mState = state;
        }
    } // namespace InputCapture
} // namespace Driver