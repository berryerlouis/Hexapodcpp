#pragma once


#include "ComponentInterface.h"

namespace Component {
class SensorProximityInterface : public ComponentInterface {
public:
	enum SensorsId
	{
		SRF_LEFT = 0x00U,
		SRF_RIGHT,
		VLX
	};
	static const uint8_t NB_SENSORS = 3U;
	SensorProximityInterface( void ) = default;
	~SensorProximityInterface()      = default;

	virtual Core::CoreStatus Initialize( void )              = 0;
	virtual void Update( const uint64_t currentTime )        = 0;
	virtual uint16_t GetDistance( const SensorsId sensorId ) = 0;
	virtual Core::CoreStatus SetThreshold( const SensorsId sensorId, const uint16_t threshold ) = 0;
	virtual uint16_t GetThreshold( const SensorsId sensorId ) = 0;
	virtual bool IsDetecting( const SensorsId sensorId )      = 0;
};
}
