#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../cmp/SensorProximityInterface.h"
#include "../cmp/Vl53l0x.h"

namespace Component {
class SensorProximity : public SensorProximityMultipleInterface, public SensorProximityObserverInterface {
public:
	static const uint8_t NB_SENSORS = 3U;
	SensorProximity( SensorProximityInterface &srf05Left, SensorProximityInterface &srf05Right, SensorProximityInterface &Vl53l0x );
	~SensorProximity() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;
	virtual uint16_t GetDistance( const SensorsId &sensorId ) final override;
	virtual Core::CoreStatus SetThreshold( const SensorsId &sensorId, const uint16_t threshold ) final override;
	virtual uint16_t GetThreshold( const SensorsId &sensorId ) final override;

	virtual void Detect( const SensorsId &sensorId, const uint16_t &distance ) final override;

private:
	SensorProximityInterface *mSensors[NB_SENSORS];
};
}
