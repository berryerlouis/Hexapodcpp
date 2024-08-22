#pragma once

#include <stdint.h>

namespace Misc {
namespace Geometry {
struct Position3d
{
	float x;
	float y;
	float z;
};
struct Rotation3d
{
	float angleX;
	float angleY;
	float angleZ;
};
struct Vector3
{
	int16_t x;
	int16_t y;
	int16_t z;
};
}
}
