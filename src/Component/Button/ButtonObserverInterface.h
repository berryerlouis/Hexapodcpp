#pragma once

#include "ButtonState.h"

namespace Component
{
    namespace Button
    {
        class ButtonObserverInterface {
        public:
            ButtonObserverInterface() = default;

            ~ButtonObserverInterface() = default;

            virtual void UpdatedButtonState(const ButtonState &ButtonState, const uint16_t period) = 0;
        };
    }
}
