#pragma once

#include <stdint.h>

class SensorInterface {
public:
	enum SensorsId
	{
		SRF_LEFT = 0x00U,
		SRF_RIGHT,
		VLX
	};
	SensorInterface(void) = default;
	~SensorInterface()    = default;

	virtual Core::CoreStatus Initialize(void)              = 0;
	virtual void Update(const uint32_t currentTime)        = 0;
	virtual uint16_t GetDistance(const SensorsId sensorId) = 0;
	virtual Core::CoreStatus SetThreshold(const SensorsId sensorId, const uint16_t threshold) = 0;
	virtual Core::CoreStatus IsDetecting(const SensorsId sensorId) = 0;
};
