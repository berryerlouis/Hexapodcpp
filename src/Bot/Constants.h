#pragma once

#include <stdint.h>

namespace Bot
{
    enum ELeg {
        FRONT_LEFT = 0x00,
        MIDDLE_LEFT,
        REAR_LEFT,
        FRONT_RIGHT,
        MIDDLE_RIGHT,
        REAR_RIGHT
    };

    static constexpr uint8_t NB_LEGS = 6U;
}
