#pragma once

#include "../clu/ClusterBattery.h"
#include "Service.h"


using namespace Component;
class ServiceBattery : public Service {
public:
	ServiceBattery( ClusterBattery &clusterBattery );
	~ServiceBattery() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint32_t currentTime ) final override;

protected:
	ClusterBattery &mClusterBattery;
	BatteryInterface::BatteryState mCurrentState;
};
