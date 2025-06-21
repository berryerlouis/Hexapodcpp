#include "Led.h"

namespace Component
{
    namespace Led
    {
        Led::Led(Driver::Gpio::GpioInterface &gpio) :
            mGpio(gpio)
            , mState(OFF) {
        }

        Core::Status Led::Initialize(void) {
            LOG_COMPONENT_DEBUG("Led", "led %d Initialized.", this->mGpio.GetPin().pin);
            this->Off();
            return (Core::Status::CORE_OK);
        }

        void Led::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        Core::Status Led::On() {
            this->mState = ON;
            return (this->mGpio.Set());
        }

        Core::Status Led::Off() {
            this->mState = OFF;
            return (this->mGpio.Reset());
        }

        Core::Status Led::Toggle() {
            if (ON == this->Get()) {
                this->Off();
            } else {
                this->On();
            }
            return (Core::Status::CORE_OK);
        }

        Led::LedState Led::Get() const {
            return (this->mState);
        }
    }
}
