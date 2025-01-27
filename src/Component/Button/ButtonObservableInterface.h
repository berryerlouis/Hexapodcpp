#pragma once

#include "ButtonObserverInterface.h"

namespace Component
{
    namespace Button
    {
        class ButtonObservableInterface {
        public:
            ButtonObservableInterface() = default;

            ~ButtonObservableInterface() = default;

            virtual Core::Status Attach(ButtonObserverInterface *observer) = 0;

            virtual void Notify(const ButtonState &state, const uint16_t period) = 0;
        };
    }
}
