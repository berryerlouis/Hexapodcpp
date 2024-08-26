#pragma once

#include "SensorsId.h"

namespace Component {
namespace Proximity {
class SensorProximityObserverInterface {
public:
	SensorProximityObserverInterface()  = default;
	~SensorProximityObserverInterface() = default;

	virtual void Detect( const SensorsId &sensorId ) = 0;
};
}
}
