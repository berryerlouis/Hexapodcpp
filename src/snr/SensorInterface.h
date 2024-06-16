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

	virtual bool Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime)        = 0;
	virtual uint16_t GetDistance(const SensorsId sensorId) = 0;
	virtual bool SetThreshold(const SensorsId sensorId, const uint16_t threshold) = 0;
	virtual bool IsDetecting(const SensorsId sensorId) = 0;
};
