#pragma once

#include <stdint.h>

class SensorInterface {
public:
	SensorInterface(void) = default;
	~SensorInterface()    = default;

	virtual bool Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;
};
