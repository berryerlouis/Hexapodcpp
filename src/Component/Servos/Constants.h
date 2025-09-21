#pragma once

#include "stdint.h"

namespace Component
{
    namespace Servos
    {
        static const uint8_t NB_SERVOS = 18U;

        enum EServos : uint8_t {
            SERVO_0 = 0U,
            SERVO_1,
            SERVO_2,
            SERVO_3,
            SERVO_4,
            SERVO_5,
            SERVO_6,
            SERVO_7,
            SERVO_8,
            SERVO_9,
            SERVO_10,
            SERVO_11,
            SERVO_12,
            SERVO_13,
            SERVO_14,
            SERVO_15,
            SERVO_16,
            SERVO_17,
        };

    }
}
