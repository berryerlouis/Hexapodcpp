#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace LedPwm
    {
        class LedPwmInterface : public ComponentInterface<0U, nullptr_t> {
        public:
            LedPwmInterface() = default;

            ~LedPwmInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual void Stop() = 0;

            virtual void SetDutyCycle(const uint16_t duty) = 0;

            virtual void FadeIn(const uint16_t duration) = 0;

            virtual void FadeOut(const uint16_t duration) = 0;
        };
    }
}