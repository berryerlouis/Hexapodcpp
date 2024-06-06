#pragma once

#include "../cmp/Servos.h"
#include "Service.h"

class ServiceControl : public Service {
public:
	ServiceControl(Servos &servos);
	~ServiceControl() = default;

	virtual void Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	Servos &mServos;
	uint8_t mStepPca9685;
};
