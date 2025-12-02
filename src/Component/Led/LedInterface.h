#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace Led
    {
        class LedInterface : public ComponentInterface<0U, nullptr_t> {
        public:
            enum LedState {
                ON = 0,
                OFF
            };

            LedInterface() = default;

            ~LedInterface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual Core::Status On() = 0;

            virtual Core::Status Off() = 0;

            virtual Core::Status Toggle() = 0;

            virtual LedState Get() const = 0;
        };
    }
}