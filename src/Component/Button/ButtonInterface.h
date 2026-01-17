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

            virtual ButtonState Get() const = 0;
        };
    } // namespace Button
} // namespace Component
