#pragma once

#include "../cmp/BatteryInterface.h"
#include "../clu/ClusterBattery.h"
#include "Service.h"


using namespace Component;
class ServiceBattery : public Service, public BatteryObserverInterface
{
public:
	ServiceBattery( ClusterBattery &clusterBattery, BatteryInterface &batteryInterface );
	~ServiceBattery() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void UpdatedBatteryState( const BatteryState &batteryState ) final override;

protected:
	ClusterBattery &mClusterBattery;
	BatteryInterface &mBatteryInterface;
};
