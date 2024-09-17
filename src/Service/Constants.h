#pragma once

#include <stdint.h>

namespace Service
{
    enum EServices {
        BATTERY = 0U,
        CONTROL = 1U,
        COMMUNICATION = 2U,
        DISPLAY = 3U,
        GENERAL = 4U,
        ORIENTATION = 5U,
        PROXIMITY = 6U
    };

    static constexpr uint8_t NB_SERVICES = 7U;
}
