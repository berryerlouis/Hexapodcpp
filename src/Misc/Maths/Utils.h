#pragma once

#include <math.h>
#include <stdint.h>

#include "Geometry.h"

namespace Misc
{
    namespace Utils
    {
        static inline float ToDeg(const float a) {
            return a * 180.0F / M_PI;
        }

        static inline float ToRad(const float a) {
            return a / 180.0F * M_PI;
        }

        template<typename T>
        static inline T Lerp(const T a, const T b, const T t) {
            return a + t * (b - a);
        }

        static inline Maths::Position3d LerpPosition(const Maths::Position3d a,
                                                     const Maths::Position3d b,
                                                     const float t) {
            return {Lerp<float>(a.x, b.x, t),
                    Lerp<float>(a.y, b.y, t),
                    Lerp<float>(a.z, b.z, t)};
        }

        static inline Maths::Position3d QuadraticLerp(const Maths::Position3d a,
                                                      const Maths::Position3d b,
                                                      const Maths::Position3d c,
                                                      const float t) {
            float u = 1.0F - t;
            return {u * u * a.x + 2 * u * t * b.x + t * t * c.x,
                    u * u * a.y + 2 * u * t * b.y + t * t * c.y,
                    u * u * a.z + 2 * u * t * b.z + t * t * c.z};
        }

        static inline Maths::Position3d
        bezierQuadratic(const Maths::Position3d &p0,
                        const Maths::Position3d &p1,
                        const Maths::Position3d &p2,
                        const double             t) {
            double            u = 1 - t;
            Maths::Position3d result;
            result.x = u * u * p0.x + 2 * u * t * p1.x + t * t * p2.x;
            result.y = u * u * p0.y + 2 * u * t * p1.y + t * t * p2.y;
            result.z = u * u * p0.z + 2 * u * t * p1.z + t * t * p2.z;
            return result;
        }


        static inline long Map(const long x,
                               const long in_min,
                               const long in_max,
                               const long out_min,
                               const long out_max) {
            return ((x - in_min) * (out_max - out_min) / (in_max - in_min) +
                    out_min);
        }

#define UINT16_TO_ARRAY(val) \
    { static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U) }
#define UINT24_TO_ARRAY(val)                                                \
    {                                                                       \
        static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), \
                static_cast<uint8_t>(val >> 16U)                            \
    }
#define UINT32_TO_ARRAY(val)                                                \
    {                                                                       \
        static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), \
                static_cast<uint8_t>(val >> 16U),                           \
                static_cast<uint8_t>(val >> 24U)                            \
    }
#define UINT40_TO_ARRAY(val)                                                \
    {                                                                       \
        static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), \
                static_cast<uint8_t>(val >> 16U),                           \
                static_cast<uint8_t>(val >> 24U),                           \
                static_cast<uint8_t>(val >> 32U)                            \
    }
#define UINT48_TO_ARRAY(val)                                                \
    {                                                                       \
        static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), \
                static_cast<uint8_t>(val >> 16U),                           \
                static_cast<uint8_t>(val >> 24U),                           \
                static_cast<uint8_t>(val >> 32U),                           \
                static_cast<uint8_t>(val >> 40U)                            \
    }
#define UINT56_TO_ARRAY(val)                                                \
    {                                                                       \
        static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), \
                static_cast<uint8_t>(val >> 16U),                           \
                static_cast<uint8_t>(val >> 24U),                           \
                static_cast<uint8_t>(val >> 32U),                           \
                static_cast<uint8_t>(val >> 40U),                           \
                static_cast<uint8_t>(val >> 48U)                            \
    }
#define UINT64_TO_ARRAY(val)                                                \
    {                                                                       \
        static_cast<uint8_t>(val & 0xFFU), static_cast<uint8_t>(val >> 8U), \
                static_cast<uint8_t>(val >> 16U),                           \
                static_cast<uint8_t>(val >> 24U),                           \
                static_cast<uint8_t>(val >> 32U),                           \
                static_cast<uint8_t>(val >> 40U),                           \
                static_cast<uint8_t>(val >> 48U),                           \
                static_cast<uint8_t>(val >> 56U)                            \
    }

#define PTR_TO_UINT16(arr) (uint16_t)((arr)[0U] | (uint16_t) (arr)[1U] << 8U)
#define PTR_TO_UINT24(arr)                                       \
    (uint32_t)(uint32_t)(arr)[0U] | (uint32_t) (arr)[1U] << 8U | \
            (uint32_t) (arr)[2U] << 16U;
#define PTR_TO_UINT32(arr)                                       \
    (uint32_t)(uint32_t)(arr)[0U] | (uint32_t) (arr)[1U] << 8U | \
            (uint32_t) (arr)[2U] << 16U | (uint32_t) (arr)[3U] << 24U;
#define PTR_TO_UINT40(arr)                                              \
    (uint32_t)(uint64_t)(arr)[0U] | (uint64_t) (arr)[1U] << 8U |        \
            (uint64_t) (arr)[2U] << 16U | (uint64_t) (arr)[3U] << 24U | \
            (uint64_t) (arr)[4U] << 32U;
#define PTR_TO_UINT48(arr)                                              \
    (uint32_t)(uint64_t)(arr)[0U] | (uint64_t) (arr)[1U] << 8U |        \
            (uint64_t) (arr)[2U] << 16U | (uint64_t) (arr)[3U] << 24U | \
            (uint64_t) (arr)[4U] << 32U | (uint64_t) (arr)[5U] << 40U;
#define PTR_TO_UINT56(arr)                                              \
    (uint32_t)(uint64_t)(arr)[0U] | (uint64_t) (arr)[1U] << 8U |        \
            (uint64_t) (arr)[2U] << 16U | (uint64_t) (arr)[3U] << 24U | \
            (uint64_t) (arr)[4U] << 32U | (uint64_t) (arr)[5U] << 40U | \
            (uint64_t) (arr)[6U] << 48U;
#define PTR_TO_UINT64(arr)                                              \
    (uint32_t)(uint64_t)(arr)[0U] | (uint64_t) (arr)[1U] << 8U |        \
            (uint64_t) (arr)[2U] << 16U | (uint64_t) (arr)[3U] << 24U | \
            (uint64_t) (arr)[4U] << 32U | (uint64_t) (arr)[5U] << 40U | \
            (uint64_t) (arr)[6U] << 48U | (uint64_t) (arr)[7U] << 56U;
    } // namespace Utils
} // namespace Misc
