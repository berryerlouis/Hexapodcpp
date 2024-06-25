#pragma once

#include "../clu/ClusterServo.h"
#include "Service.h"


using namespace Component;
class ServiceControl : public Service {
public:
	ServiceControl( ClusterServo &clusterServo );
	~ServiceControl() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

protected:
	ClusterServo &mClusterServo;
	uint8_t mStepPca9685;
};
