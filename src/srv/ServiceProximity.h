#pragma once

#include "../snr/SensorProximity.h"
#include "Service.h"

class ServiceProximity : public Service {
public:
	ServiceProximity(SensorProximity &SensorProximity);
	~ServiceProximity() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

private:
	SensorProximity &mSensorProximity;
};
