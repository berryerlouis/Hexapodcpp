#include "Button.h"

namespace Component
{
    namespace Button
    {
        static Button *button[3U] = {};
        static uint8_t buttonIndex = 0U;

        void InterruptGpioBp(void) {
            for (size_t i = 0U; i < buttonIndex; i++) {
                button[i]->Hit();
            }
        }

        Button::Button(Gpio::GpioInterface &gpio) :
                                                  mGpioButton(gpio)
                                                  , mPushTime(0U)
                                                  , mState(RELEASE) {
            button[buttonIndex] = this;
            buttonIndex++;
        }

        Core::Status Button::Initialize(void) {
            this->mGpioButton.SetInterruptPin(&InterruptGpioBp);
            LOG_COMPONENT_DEBUG("Button", "Initialized.");
            return (Core::Status::CORE_OK);
        }

        void Button::Hit(void) {
            if (this->mGpioButton.Get() == true) {
                this->mState = PUSH;
                this->mPushTime = Tick::Tick::GetInstance().GetUs();
                this->Notify({this->mState, 0U});
            } else {
                if (this->mState == PUSH) {
                    this->mState = RELEASE;
                    const uint64_t delayMs = Tick::Tick::GetInstance().GetUs() - this->mPushTime;
                    this->Notify({this->mState, delayMs});
                }
            }
        }

        void Button::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        ButtonState Button::Get() const {
            return (this->mState);
        }
    }
}