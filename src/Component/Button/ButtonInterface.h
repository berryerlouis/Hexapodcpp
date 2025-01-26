#pragma once

#include "ButtonObservable.h"
#include "../ComponentInterface.h"

namespace Component
{
    namespace Button
    {
        class ButtonInterface : public ComponentInterface, public Proximity::ButtonObservable {
        public:
            enum ButtonState {
                RELEASE = 0,
                PUSH
            };

            ButtonInterface() = default;

            ~ButtonInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual ButtonState Get() const = 0;
        };
    }
}
