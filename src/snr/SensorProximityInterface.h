#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "SensorInterface.h"

class SensorProximityInterface : public SensorInterface {
public:
	enum SensorsId
	{
		SRF_LEFT = 0x00U,
		SRF_RIGHT,
		VLX
	};
	SensorProximityInterface(void) = default;
	~SensorProximityInterface()    = default;

	virtual uint16_t GetDistance(const SensorsId sensorId) = 0U;
	virtual bool SetThreshold(const SensorsId sensorId, const uint16_t threshold) = 0U;
	virtual bool IsDetecting(const SensorsId sensorId) = 0U;

private:
};
