#pragma once

#include "DriverInterface.h"

class Driver : public DriverInterface {
public:
	Driver(uint32_t updateTime)
		: mUpdateTime(updateTime)
		, mPreviousTime(0UL) {
	}

	~Driver() = default;

	virtual void Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;

	volatile uint32_t mUpdateTime;
	volatile uint32_t mPreviousTime;
};
