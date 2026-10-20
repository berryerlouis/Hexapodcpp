#pragma once

#include <atomic>
#include "../../Driver/Gpio/GpioInterface.h"
#include "ButtonInterface.h"

namespace Component
{
    namespace Button
    {
        using namespace Driver;

        class Button : public ButtonInterface {
        public:
            explicit Button(Driver::Gpio::GpioInterface &gpio);

            ~Button() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual ButtonState  Get(void) const final override;


            void                 Hit(void);

        private:
            Driver::Gpio::GpioInterface &mGpioButton;
            uint64_t                     mPushTime;
            // Edges are recorded in the interrupt thread and notified from Update().
            std::atomic<bool>            mPushPending;
            std::atomic<bool>            mReleasePending;
            std::atomic<uint32_t>        mReleaseDelayUs;
            ButtonState                  mNotifiedState;

        public:
            std::atomic<ButtonState> mState;
        };
    } // namespace Button
} // namespace Component
