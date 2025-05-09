#pragma once

#include <stdint.h>
#include <math.h>

namespace Misc
{
    namespace Maths
    {
        struct Position3d {
            float x;
            float y;
            float z;

            bool
            operator==(const Position3d &other) const {
                return (x == other.x && y == other.y && z == other.z);
            }

            Position3d
            operator*=(const Position3d &value) {
                x = x * value.x;
                y = y * value.y;
                z = z * value.z;
                return *this;
            }

            Position3d
            operator*(const float &value) {
                x = x * value;
                y = y * value;
                z = z * value;
                return *this;
            }

            Position3d
            operator/=(const Position3d &value) {
                x = x / value.x;
                y = y / value.y;
                z = z / value.z;
                return *this;
            }

            Position3d
            operator/(const float &value) {
                x = x / value;
                y = y / value;
                z = z / value;
                return *this;
            }

            Position3d
            operator+=(const Position3d &value) {
                x = x + value.x;
                y = y + value.y;
                z = z + value.z;
                return *this;
            }

            Position3d
            operator+(const float &value) {
                x = x + value;
                y = y + value;
                z = z + value;
                return *this;
            }

            Position3d
            operator-=(const Position3d &value) {
                x = x - value.x;
                y = y - value.y;
                z = z - value.z;
                return *this;
            }

            Position3d
            operator-(const float &value) {
                x = x - value;
                y = y - value;
                z = z - value;
                return *this;
            }
        };

        struct Rotation3d {
            float angleX;
            float angleY;
            float angleZ;

            bool
            operator==(const Rotation3d &other) const {
                return (angleX == other.angleX && angleY == other.angleY && angleZ == other.angleZ);
            }
        };

        struct Vector3 {
            int16_t x;
            int16_t y;
            int16_t z;
        };


        struct Vector3F {
            float x;
            float y;
            float z;
        };

        struct Position3D {
            float yaw;
            float pitch;
            float roll;
        };
    }
}
