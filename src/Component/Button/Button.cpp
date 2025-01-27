#include "Button.h"

namespace Component
{
    namespace Button
    {
        Button::Button(Driver::InputCapture::InputCaptureInterface &inputCapture) :
            mInputCaptureButton(inputCapture)
            , mState(RELEASE) {
        }

        Core::Status Button::Initialize(void) {
            return (this->mInputCaptureButton.Initialize());
        }

        void Button::Update(const uint64_t currentTime) {
            (void) currentTime;
            const uint64_t time = this->mInputCaptureButton.GetInputCaptureTime();
            if (time > 50) {
                this->Notify(this->mState, time);
            }
        }

        ButtonState Button::Get() const {
            return (this->mState);
        }

        Core::Status Button::Attach(ButtonObserverInterface *observer) {
            return (this->mObservable.Attach(observer));
        }

        void Button::Notify(const ButtonState &state, const uint16_t voltage) {
            this->mObservable.Notify(state, voltage);
        }

    }
}
