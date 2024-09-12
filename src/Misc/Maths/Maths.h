#pragma once

#include <stdint.h>
#include <math.h>

namespace Misc
{
    namespace Maths
    {
        static inline uint8_t Lerp(const uint8_t a, const uint8_t b, const float t) {
            return a + (b - a) * t;
        }

        static inline long Map(const long x, const long in_min, const long in_max, const long out_min,
                               const long out_max) {
            return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
        }
    }
}
