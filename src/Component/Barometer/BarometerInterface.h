#pragma once

#include "../ComponentInterface.h"

namespace Component
{
    namespace Barometer
    {
        enum BarometerResolution {
            MS5611_ULTRA_HIGH_RES = 0x08U,
            MS5611_HIGH_RES = 0x06U,
            MS5611_STANDARD = 0x04U,
            MS5611_LOW_POWER = 0x02U,
            MS5611_ULTRA_LOW_POWER = 0x00U
        };

        class BarometerInterface : public ComponentInterface<0U, std::nullptr_t> {
        public:
            BarometerInterface() = default;

            ~BarometerInterface() = default;

            virtual int16_t  GetTemp(void) = 0;

            virtual int32_t  GetPressure(void) = 0;

            virtual uint16_t GetAltitude(void) = 0;
        };
    } // namespace Barometer
} // namespace Component
