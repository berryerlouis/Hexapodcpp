#pragma once

#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../Service.h"

namespace Service {
namespace Proximity {
using namespace Component::Proximity;

class ServiceProximity : public Service, public SensorProximityObserverInterface {
public:
	ServiceProximity( SensorProximityMultipleInterface &proximity );

	~ServiceProximity() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void Detect( const SensorsId &sensorId ) final override;

protected:
	SensorProximityMultipleInterface &mProximity;
};
}
}
