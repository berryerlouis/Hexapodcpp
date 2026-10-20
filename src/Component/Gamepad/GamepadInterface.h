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
            // Both observable bases declare Attach/Notify; expose them as overloads to avoid ambiguous lookup.
            using ComponentInterface<3U, GamepadButtonState>::Attach;
            using Core::Observable<3U, GamepadConnectedState>::Attach;

            GamepadInterface()  = default;
            ~GamepadInterface() = default;

            virtual const GamepadButtonState    &GetButtonState(void) const = 0;

            virtual const GamepadConnectedState &GetConnectedState(void) const = 0;

            virtual bool                        IsConnected(void) const = 0;

        protected:
            using ComponentInterface<3U, GamepadButtonState>::Notify;
            using Core::Observable<3U, GamepadConnectedState>::Notify;
        };
    } // namespace Gamepad
} // namespace Component
