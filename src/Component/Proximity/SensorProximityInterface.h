#pragma once

#include "../ComponentInterface.h"
#include "../ComponentObservable.h"
#include "../ComponentObserverInterface.h"

namespace Component {
namespace Proximity {
static const uint8_t NB_SENSORS = 3U;
enum SensorsId
{
	SRF_LEFT = 0x00U,
	SRF_RIGHT,
	VLX
};
class SensorProximityInterface : public ComponentInterface, public ComponentObservable2 <SensorsId, uint16_t>  {
public:
	SensorProximityInterface( void ) = default;
	~SensorProximityInterface()      = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual uint16_t GetDistance( void )  = 0;
	virtual uint16_t GetThreshold( void ) = 0;
	virtual Core::CoreStatus SetThreshold( const uint16_t threshold ) = 0;
};

class SensorProximityMultipleInterface : public ComponentInterface, public ComponentObservable2 <SensorsId, uint16_t>  {
public:
	SensorProximityMultipleInterface( void ) = default;
	~SensorProximityMultipleInterface()      = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual uint16_t GetDistance( const SensorsId &sensorId )  = 0;
	virtual uint16_t GetThreshold( const SensorsId &sensorId ) = 0;
	virtual Core::CoreStatus SetThreshold( const SensorsId &sensorId, const uint16_t threshold ) = 0;
};

class SensorProximityObserverInterface : public ComponentObserver2Interface <SensorsId, uint16_t> {
public:
	SensorProximityObserverInterface()  = default;
	~SensorProximityObserverInterface() = default;

	virtual void Detect( const SensorsId &sensorId, const uint16_t &distance ) = 0;

	inline virtual void UpdatedValue ( const SensorsId &sensorId, const uint16_t &distance ) final override
	{
		Detect( sensorId, distance );
	}
};
}
}