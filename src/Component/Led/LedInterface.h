#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace Led
    {
        class LedInterface : public ComponentInterface<0U, std::nullptr_t> {
        public:
            enum LedState {
                ON = 0,
                OFF
            };

            LedInterface() = default;

            ~LedInterface() = default;

            virtual Core::Status On() = 0;

            virtual Core::Status Off() = 0;

            virtual Core::Status Toggle() = 0;

            virtual Core::Status Pwm(const uint16_t delay) = 0;

            virtual LedState     Get() const = 0;
        };
    } // namespace Led
} // namespace Component
