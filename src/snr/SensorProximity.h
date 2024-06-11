#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../cmp/Srf05.h"
#include "../cmp/Vl53l0x.h"
#include "SensorInterface.h"

class SensorProximity : public SensorInterface {
public:
	enum SensorsId
	{
		SRF_LEFT = 0x00U,
		SRF_RIGHT,
		VLX
	};
	static const uint8_t NB_SENSORS = 3U;
	SensorProximity(Srf05 &srf05Left, Srf05 &srf05Right, Vl53l0x &Vl53l0x);
	~SensorProximity() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;
	uint16_t GetDistance(const SensorsId sensorId);
	bool SetThreshold(const SensorsId sensorId, const uint16_t threshold);
	bool IsDetecting(const SensorsId sensorId);

	bool BuildFrameDistance(EProximityCommands side, Frame &response);

private:
	static const uint16_t UPDATE_STEP_SRF_MS = 1U;
	static const uint16_t UPDATE_STEP_VLX_MS = 5U;
	Srf05 &mSrf05Left;
	Srf05 &mSrf05Right;
	Vl53l0x &mVl53l0x;
	uint8_t mStepSrf;
	uint8_t mStepVlx;
};
