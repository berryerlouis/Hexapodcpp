#include "Button.h"

namespace Component
{
    namespace Button
    {
        static Button *button[3U] = {};
        static uint8_t buttonIndex = 0U;

        void           InterruptGpioBp() {
            for (size_t i = 0U; i < buttonIndex; i++) {
                button[i]->Hit();
            }
        }

        Button::Button(Gpio::GpioInterface &gpio)
            : mGpioButton(gpio)
            , mPushTime(0U)
            , mPushPending(false)
            , mReleasePending(false)
            , mReleaseDelayUs(0U)
            , mNotifiedState(RELEASE)
            , mState(RELEASE) {
            button[buttonIndex] = this;
            buttonIndex++;
        }

        Core::Status Button::Initialize() {
            this->mGpioButton.SetInterruptPin(&InterruptGpioBp);
            LOG_COMPONENT_DEBUG("Button", "Initialized.");
            return Core::Status::CORE_OK;
        }

        void Button::Hit() {
            if (this->mGpioButton.Get()) {
                this->mState = PUSH;
                this->mPushTime = Timer::Tick::GetInstance().GetUs();
                this->mPushPending = true;
            } else {
                if (this->mState == PUSH) {
                    this->mState = RELEASE;
                    const uint64_t delayUs = Timer::Tick::GetInstance().GetUs() - this->mPushTime;
                    this->mReleaseDelayUs = static_cast<uint32_t>(
                            delayUs > UINT32_MAX ? UINT32_MAX : delayUs);
                    this->mReleasePending = true;
                }
            }
        }

        void Button::Update(const uint64_t currentTime) {
            (void) currentTime;
            if (this->mPushPending.exchange(false)) {
                this->mNotifiedState = PUSH;
                this->Notify({this->mNotifiedState, 0U});
            }
            if (this->mReleasePending.exchange(false)) {
                this->mNotifiedState = RELEASE;
                this->Notify({this->mNotifiedState, this->mReleaseDelayUs.load()});
            }
        }

        ButtonState Button::Get() const {
            return this->mState;
        }
    } // namespace Button
} // namespace Component
