#pragma once

#include "../ComponentInterface.h"
#include "../../Core/Observable.h"
#include "GamepadState.h"

namespace Component
{
    namespace Gamepad
    {
        class GamepadInterface : public ComponentInterface<3U, GamepadButtonState>,
                                 public Core::Observable<3U, GamepadConnectedState> {
        public:
            GamepadInterface()  = default;
            ~GamepadInterface() = default;

            virtual const GamepadButtonState    &GetButtonState(void) const = 0;

            virtual const GamepadConnectedState &GetConnectedState(void) const = 0;

            virtual bool                        IsConnected(void) const = 0;
        };
    } // namespace Gamepad
} // namespace Component
