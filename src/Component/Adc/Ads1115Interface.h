#pragma once

#include "../../Core/Observable.h"
#include "../ComponentInterface.h"
#include "Ads1115Id.h"

namespace Component
{
    namespace Adc
    {
        class Ads1115Interface : public ComponentInterface<0U, std::nullptr_t> {
        public:
            Ads1115Interface() = default;

            ~Ads1115Interface() = default;

            virtual float ReadADC(const Ads1115Pin pin) const = 0;
        };
    } // namespace Adc
} // namespace Component
