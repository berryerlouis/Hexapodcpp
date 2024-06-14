#pragma once

#include "ComponentInterface.h"

namespace Component {
class Mpu9150Interface : public ComponentInterface {
public:
	struct Vector3
	{
		int16_t x;
		int16_t y;
		int16_t z;
	};

	Mpu9150Interface(void) = default;
	~Mpu9150Interface()    = default;

	virtual Vector3 ReadAcc(void)  = 0;
	virtual Vector3 ReadGyr(void)  = 0;
	virtual Vector3 ReadMag(void)  = 0;
	virtual int16_t ReadTemp(void) = 0;
};
}
