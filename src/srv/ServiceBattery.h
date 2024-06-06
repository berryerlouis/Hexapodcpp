#pragma once

#include "../cmp/Battery.h"
#include "Service.h"

class ServiceBattery : public Service {
public:
	ServiceBattery(Battery &battery);
	~ServiceBattery() = default;

	virtual void Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	Battery &mBattery;
};
