#pragma once

#include "../../Driver/Gpio/GpioInterface.h"
#include "ButtonInterface.h"

namespace Component
{
    namespace Button
    {
        using namespace Driver;

        class Button : public ButtonInterface {
        public:
            Button(Gpio::GpioInterface &gpio);

            ~Button() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual ButtonState  Get(void) const final override;


            void                 Hit(void);

        private:
            Gpio::GpioInterface &mGpioButton;
            uint64_t             mPushTime;

        public:
            ButtonState mState;
        };
    } // namespace Button
} // namespace Component
