#pragma once

#include "ComponentInterface.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../drv/Adc.h"

namespace Component {
using namespace Driver;
using namespace Cluster;

class Battery : public ComponentInterface {
	enum BatteryState
	{
		NOMINAL = 0x00U,
		WARNING,
		CRITICAL,
		UNKNOWN = 0xFFU
	};

public:
	Battery(const SGpio &gpio);
	~Battery() = default;

	BatteryState GetState(void);
	uint16_t     GetVoltage(void);

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	bool BuildFrameVoltage(Frame & response);
	bool BuildFrameState(Frame & response);

private:
	uint16_t mVoltage;
	BatteryState mState;
	Adc mAdc;
};
}
