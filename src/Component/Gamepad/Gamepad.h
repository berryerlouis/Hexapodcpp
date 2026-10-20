#pragma once

#include "../../Driver/Joystick/JoystickInterface.h"
#include "GamepadInterface.h"

namespace Component
{
    namespace Gamepad
    {
        class Gamepad : public GamepadInterface {
        public:
            explicit Gamepad(Driver::Joystick::JoystickInterface &joystick);

            ~Gamepad() = default;

            virtual Core::Status                Initialize(void) final override;

            virtual void                        Update(const uint64_t currentTime) final override;

            virtual const GamepadButtonState    &GetButtonState(void) const final override;

            virtual const GamepadConnectedState &GetConnectedState(void) const final override;

            virtual bool                        IsConnected(void) const final override;

        private:
            Driver::Joystick::JoystickInterface &mJoystick;
            GamepadButtonState                   mButtonState;
            GamepadConnectedState                mConnectedState;
            bool                                 mConnected;
            bool                                 mDirty;
        };
    } // namespace Gamepad
} // namespace Component
