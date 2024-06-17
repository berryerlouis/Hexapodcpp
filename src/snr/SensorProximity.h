#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../cmp/ProximityInterface.h"
#include "../cmp/Vl53l0x.h"
#include "SensorInterface.h"

using namespace Component;
class SensorProximity : public SensorInterface {
public:
	static const uint8_t NB_SENSORS = 3U;
	SensorProximity(ProximityInterface &srf05Left, ProximityInterface &srf05Right, ProximityInterface &Vl53l0x);
	~SensorProximity() = default;

	virtual Core::CoreStatus Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;
	virtual uint16_t GetDistance(const SensorsId sensorId) final override;
	virtual Core::CoreStatus SetThreshold(const SensorsId sensorId, const uint16_t threshold) final override;
	virtual Core::CoreStatus IsDetecting(const SensorsId sensorId) final override;

	Core::CoreStatus BuildFrameDistance(Cluster::EProximityCommands side, Cluster::Frame &response);

private:
	static const uint16_t UPDATE_STEP_SRF_MS = 1U;
	static const uint16_t UPDATE_STEP_VLX_MS = 1U;
	ProximityInterface &mSrf05Left;
	ProximityInterface &mSrf05Right;
	ProximityInterface &mVl53l0x;
	uint8_t mStepSrf;
	uint8_t mStepVlx;
};
