#pragma once

#include <stdint.h>
#include <math.h>

namespace Misc
{
    namespace Utils
    {
        static inline uint8_t Lerp(const uint8_t a, const uint8_t b, const float t) {
            return a + (b - a) * t;
        }

        static inline long Map(const long x, const long in_min, const long in_max, const long out_min,
                               const long out_max) {
            return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
        }

#define UINT16_TO_ARRAY(val) { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U) }
#define UINT24_TO_ARRAY(val) { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), static_cast<uint8_t>(val >> 16U) }
#define UINT32_TO_ARRAY(val) { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), static_cast<uint8_t>(val >> 16U), static_cast<uint8_t>(val >> 24U) }
#define UINT40_TO_ARRAY(val) { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), static_cast<uint8_t>(val >> 16U), static_cast<uint8_t>(val >> 24U), static_cast<uint8_t>(val >> 32U) }
#define UINT48_TO_ARRAY(val) { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), static_cast<uint8_t>(val >> 16U), static_cast<uint8_t>(val >> 24U), static_cast<uint8_t>(val >> 32U), static_cast<uint8_t>(val >> 40U) }
#define UINT56_TO_ARRAY(val) { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), static_cast<uint8_t>(val >> 16U), static_cast<uint8_t>(val >> 24U), static_cast<uint8_t>(val >> 32U), static_cast<uint8_t>(val >> 40U), static_cast<uint8_t>(val >> 48U) }
#define UINT64_TO_ARRAY(val) { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), static_cast<uint8_t>(val >> 16U), static_cast<uint8_t>(val >> 24U), static_cast<uint8_t>(val >> 32U), static_cast<uint8_t>(val >> 40U), static_cast<uint8_t>(val >> 48U), static_cast<uint8_t>(val >> 56U) }

#define PTR_TO_UINT16(arr) (uint16_t)(arr)[0U] | (uint16_t)(arr)[1U] << 8U;
#define PTR_TO_UINT24(arr) (uint32_t)(uint32_t)(arr)[0U] | (uint32_t)(arr)[1U] << 8U | (uint32_t)(arr)[2U] << 16U;
#define PTR_TO_UINT32(arr) (uint32_t)(uint32_t)(arr)[0U] | (uint32_t)(arr)[1U] << 8U | (uint32_t)(arr)[2U] << 16U | (uint32_t)(arr)[3U] << 24U;
#define PTR_TO_UINT40(arr) (uint32_t)(uint64_t)(arr)[0U] | (uint64_t)(arr)[1U] << 8U | (uint64_t)(arr)[2U] << 16U | (uint64_t)(arr)[3U] << 24U | (uint64_t)(arr)[4U] << 32U;
#define PTR_TO_UINT48(arr) (uint32_t)(uint64_t)(arr)[0U] | (uint64_t)(arr)[1U] << 8U | (uint64_t)(arr)[2U] << 16U | (uint64_t)(arr)[3U] << 24U | (uint64_t)(arr)[4U] << 32U | (uint64_t)(arr)[5U] << 40U;
#define PTR_TO_UINT56(arr) (uint32_t)(uint64_t)(arr)[0U] | (uint64_t)(arr)[1U] << 8U | (uint64_t)(arr)[2U] << 16U | (uint64_t)(arr)[3U] << 24U | (uint64_t)(arr)[4U] << 32U | (uint64_t)(arr)[5U] << 40U | (uint64_t)(arr)[6U] << 48U;
#define PTR_TO_UINT64(arr) (uint32_t)(uint64_t)(arr)[0U] | (uint64_t)(arr)[1U] << 8U | (uint64_t)(arr)[2U] << 16U | (uint64_t)(arr)[3U] << 24U | (uint64_t)(arr)[4U] << 32U | (uint64_t)(arr)[5U] << 40U | (uint64_t)(arr)[6U] << 48U | (uint64_t)(arr)[7U] << 56U;
    }
}
