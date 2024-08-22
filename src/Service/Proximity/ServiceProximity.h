#pragma once

#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../../Cluster/Proximity/ClusterProximity.h"
#include "../../Cluster/Constants.h"
#include "../Service.h"

namespace Service {
namespace Proximity {
using namespace Component::Proximity;
using namespace Cluster::Proximity;

class ServiceProximity : public Service, public SensorProximityObserverInterface {
public:
<<<<<<< HEAD
<<<<<<< HEAD
	ServiceProximity( ClusterProximity &clusterProximity, SensorProximityMultipleInterface &proximity );
=======
	ServiceProximity( ClusterProximity &clusterProximity, SensorProximityInterface &proximity );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
=======
	ServiceProximity( ClusterProximity &clusterProximity, SensorProximityMultipleInterface &proximity );
>>>>>>> 4ea6771 (create observers and update UT)
	~ServiceProximity() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void Detect( const SensorsId &sensorId, const uint16_t &distance ) final override;

protected:
	ClusterProximity &mClusterProximity;
<<<<<<< HEAD
<<<<<<< HEAD
	SensorProximityMultipleInterface &mProximity;
=======
	SensorProximityInterface &mProximity;
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
=======
	SensorProximityMultipleInterface &mProximity;
>>>>>>> 4ea6771 (create observers and update UT)
};
}
}
