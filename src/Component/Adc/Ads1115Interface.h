#pragma once

#include "../ComponentInterface.h"
#include "Ads1115Id.h"
#include "../../Core/Observable.h"

namespace Component
{
    namespace Adc
    {
        class Ads1115Interface : public ComponentInterface<0U, nullptr_t> {
        public:
            Ads1115Interface() = default;

            ~Ads1115Interface() = default;

            virtual Core::Status Initialize(void) = 0;

            virtual void Update(const uint64_t currentTime) = 0;

            virtual float ReadADC(const Ads1115Pin pin) const = 0;
        };
    } // namespace Adc
} // namespace Driver
