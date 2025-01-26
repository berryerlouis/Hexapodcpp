#pragma once

#include "ButtonInterface.h"
#include "../../Driver/InputCapture/InputCaptureInterface.h"

namespace Component
{
    namespace Button
    {
        class Button : public ButtonInterface {
        public:
            Button(Driver::InputCapture::InputCaptureInterface &inputCapture);

            ~Button() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual ButtonState Get() const final override;

        private:
            Driver::InputCapture::InputCaptureInterface &mInputCapture;
            ButtonState mState;
        };
    }
}
