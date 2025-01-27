#pragma once

#include "ButtonObservable.h"
#include "../ComponentInterface.h"

namespace Component
{
    namespace Button
    {

        class ButtonInterface : public ComponentInterface, public ButtonObservableInterface {
        public:
            ButtonInterface() = default;

            ~ButtonInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual ButtonState Get() const = 0;

            virtual Core::Status Attach(ButtonObserverInterface *observer) = 0;

            virtual void Notify(const ButtonState &state, const uint16_t period) = 0;
        };
    }
}
