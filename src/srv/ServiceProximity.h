#pragma once

#include "../cmp/SensorProximityInterface.h"
#include "../clu/ClusterProximity.h"
#include "Service.h"


using namespace Component;
class ServiceProximity : public Service, public SensorProximityObserverInterface {
public:
	ServiceProximity( ClusterProximity &clusterProximity, SensorProximityMultipleInterface &proximity );
	~ServiceProximity() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void Detect( const SensorsId &sensorId, const uint16_t &distance ) final override;

protected:
	ClusterProximity &mClusterProximity;
	SensorProximityMultipleInterface &mProximity;
};
