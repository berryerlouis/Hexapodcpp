#include "Button.h"

namespace Component
{
    namespace Button
    {
        Button::Button(Driver::InputCapture::InputCaptureInterface &inputCapture) :
            mInputCapture(inputCapture)
            , mState(Button::ButtonState::RELEASE) {
        }

        Core::Status Button::Initialize(void) {
            return (Core::Status::CORE_OK);
        }

        void Button::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        Button::ButtonState Button::Get() const {
            return (this->mState);
        }
    }
}
