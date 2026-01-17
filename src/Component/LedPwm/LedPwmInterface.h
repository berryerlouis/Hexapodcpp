#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace LedPwm
    {
        class LedPwmInterface : public ComponentInterface<0U, std::nullptr_t> {
        public:
            LedPwmInterface() = default;

            ~LedPwmInterface() = default;

            virtual void Stop(void) = 0;

            virtual void UpdateFrequency(const float frequency) = 0;
        };
    } // namespace LedPwm
} // namespace Component
