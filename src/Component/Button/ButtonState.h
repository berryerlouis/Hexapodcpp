#pragma once

#include <cstdint>

namespace Component
{
    namespace Button
    {
        enum ButtonState {
            RELEASE = 0U,
            PUSH
        };

        struct ButtonStruct {
            const ButtonState &state;
            const uint64_t     delayMs;
        };
    } // namespace Button
} // namespace Component
