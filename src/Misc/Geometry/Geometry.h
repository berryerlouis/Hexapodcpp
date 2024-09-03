#pragma once

#include <stdint.h>

namespace Misc {
namespace Geometry {
struct Position3d
{
    float x;
    float y;
    float z;
    bool operator== ( const Position3d & other ) const
    {
        return ( x == other.x && y == other.y && z == other.z );
    }
};
struct Rotation3d
{
    float angleX;
    float angleY;
    float angleZ;
    bool operator== ( const Rotation3d & other ) const
    {
        return ( angleX == other.angleX && angleY == other.angleY && angleZ == other.angleZ );
    }
};
struct Vector3
{
    int16_t x;
    int16_t y;
    int16_t z;
};
}
}
