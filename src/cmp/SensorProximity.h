#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../cmp/ProximityInterface.h"
#include "../cmp/Vl53l0x.h"
#include "../cmp/SensorProximityInterface.h"

namespace Component {
class SensorProximity : public SensorProximityInterface {
public:
	static const uint8_t NB_SENSORS = 3U;
	SensorProximity( ProximityInterface &srf05Left, ProximityInterface &srf05Right, ProximityInterface &Vl53l0x );
	~SensorProximity() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;
	virtual uint16_t GetDistance( const SensorsId sensorId ) final override;
	virtual Core::CoreStatus SetThreshold( const SensorsId sensorId, const uint16_t threshold ) final override;
	virtual uint16_t GetThreshold( const SensorsId sensorId ) final override;
	virtual bool IsDetecting( const SensorsId sensorId ) final override;

private:
	static const uint16_t UPDATE_STEP_SRF_MS = 1U;
	static const uint16_t UPDATE_STEP_VLX_MS = 1U;
	ProximityInterface &mSrf05Left;
	ProximityInterface &mSrf05Right;
	ProximityInterface &mVl53l0x;
	uint8_t mStepSrf;
	uint8_t mStepVlx;
};
}
