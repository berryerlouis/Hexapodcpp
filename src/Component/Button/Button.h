#pragma once

#include "../../Driver/InputCapture/InputCaptureInterface.h"
#include "ButtonInterface.h"
#include "ButtonObservable.h"

namespace Component
{
    namespace Button
    {
        using namespace Driver;

        class Button : public ButtonInterface {
        public:
            Button(Driver::InputCapture::InputCaptureInterface &inputCapture);

            ~Button() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual ButtonState Get(void) const final override;

            virtual Core::Status Attach(ButtonObserverInterface *observer) final override;

            virtual void Notify(const ButtonState &state, const uint16_t voltage) final override;

        private:
            ButtonState mState;
            Driver::InputCapture::InputCaptureInterface &mInputCaptureButton;
            ButtonObservable mObservable;
        };
    }
}
