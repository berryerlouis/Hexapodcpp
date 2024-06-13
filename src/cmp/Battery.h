#pragma once

#include "ComponentInterface.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../drv/AdcInterface.h"

namespace Component {
using namespace Driver;
using namespace Cluster;

class Battery : public ComponentInterface {
public:
	enum BatteryState
	{
		NOMINAL = 0x00U,
		WARNING,
		CRITICAL,
		UNKNOWN = 0xFFU
	};
public:
	Battery(AdcInterface &adc);
	~Battery() = default;

	BatteryState GetState(void);
	uint16_t     GetVoltage(void);

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	uint16_t mVoltage;
	BatteryState mState;
	AdcInterface &mAdc;
};
}
