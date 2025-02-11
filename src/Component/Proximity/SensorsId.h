#pragma once

#include <stdint.h>

namespace Component
{
    namespace Proximity
    {
        static constexpr uint8_t NB_SENSORS = 3U;

        enum SensorsId {
            SRF_LEFT = 0x00U,
            SRF_RIGHT,
            VLX
        };

        struct SensorsStruct {
            const SensorsId &id;
            const uint16_t distance;
        };
    }
}
