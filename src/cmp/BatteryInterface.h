#pragma once

#include "ComponentInterface.h"

namespace Component {
class BatteryInterface : public ComponentInterface {
public:
	enum BatteryState
	{
		NOMINAL = 0x00U,
		WARNING,
		CRITICAL,
		UNKNOWN = 0xFFU
	};

	BatteryInterface(void) = default;
	~BatteryInterface()    = default;

	virtual BatteryState GetState() = 0;
	virtual uint16_t GetVoltage()   = 0;
};
}
