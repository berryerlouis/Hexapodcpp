#pragma once

#include "../ComponentInterface.h"
#include "ButtonState.h"

namespace Component
{
    namespace Button
    {
        class ButtonInterface : public ComponentInterface<5U, ButtonStruct> {
        public:
            ButtonInterface() = default;

            ~ButtonInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void         Update(const uint64_t currentTime) = 0;

            virtual ButtonState  Get() const = 0;
        };
    } // namespace Button
} // namespace Component
