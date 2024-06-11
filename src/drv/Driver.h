#pragma once

#include "DriverInterface.h"

class Driver : public DriverInterface {
public:
	Driver(uint32_t updateTime)
		: mUpdateTime(updateTime)
		, mPreviousTime(0UL)
	{
	}

	~Driver() = default;

	volatile uint32_t mUpdateTime;
	volatile uint32_t mPreviousTime;
};
