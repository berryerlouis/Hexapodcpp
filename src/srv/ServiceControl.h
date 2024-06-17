#pragma once

#include "../cmp/ServosInterface.h"
#include "Service.h"


using namespace Component;
class ServiceControl : public Service {
public:
	ServiceControl(ServosInterface &servos);
	~ServiceControl() = default;

	virtual Core::CoreStatus Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	ServosInterface &mServos;
	uint8_t mStepPca9685;
};
