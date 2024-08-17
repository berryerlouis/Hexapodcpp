#pragma once

#include "../cmp/SensorProximityInterface.h"
#include "../clu/ClusterProximity.h"
#include "Service.h"


using namespace Component;
class ServiceProximity : public Service, public SensorProximityObserverInterface {
public:
<<<<<<< HEAD
	ServiceProximity( ClusterProximity &clusterProximity, SensorProximityMultipleInterface &proximity );
=======
	ServiceProximity( ClusterProximity &clusterProximity, SensorProximityInterface &proximity );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
	~ServiceProximity() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void Detect( const SensorsId &sensorId, const uint16_t &distance ) final override;

protected:
	ClusterProximity &mClusterProximity;
<<<<<<< HEAD
	SensorProximityMultipleInterface &mProximity;
=======
	SensorProximityInterface &mProximity;
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
};
