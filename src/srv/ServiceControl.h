#pragma once

#include "../cmp/Servos.h"
#include "Service.h"


using namespace Component;
class ServiceControl : public Service {
public:
	ServiceControl(Servos &servos);
	~ServiceControl() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	Servos &mServos;
	uint8_t mStepPca9685;
};
