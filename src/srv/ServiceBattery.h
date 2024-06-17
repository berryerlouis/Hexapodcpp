#pragma once

#include "../cmp/BatteryInterface.h"
#include "Service.h"


using namespace Component;
class ServiceBattery : public Service {
public:
	ServiceBattery(BatteryInterface &battery);
	~ServiceBattery() = default;

	virtual Core::CoreStatus Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;


	Core::CoreStatus BuildFrameState(Frame &response);

private:
	BatteryInterface &mBattery;
	BatteryInterface::BatteryState mCurrentState;
};
