#pragma once

#include "ButtonObserverInterface.h"

namespace Component
{
    namespace Proximity
    {
        class ButtonObservableInterface {
        public:
            ButtonObservableInterface() = default;

            ~ButtonObservableInterface() = default;

            virtual Core::Status Attach(ButtonObserverInterface *observer) = 0;

            virtual void Notify(const uint16_t distance) = 0;
        };
    }
}
