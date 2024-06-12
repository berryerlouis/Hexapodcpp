#pragma once

#include "../clu/Constants.h"
#include "../cmp/ProximityInterface.h"
#include "../drv/InputCapture.h"
#include "../drv/Gpio.h"

namespace Component {
using namespace Driver;
using namespace Cluster;
class Srf05 : public ProximityInterface {
public:
	static const uint64_t ECHO_TIMEOUT       = 30000U;
	static const uint16_t DISTANCE_THRESHOLD = 30U;

	Srf05(const EProximityCommands side, const SGpio &gpioTrigger, const SGpio &gpioEcho);
	~Srf05() = default;

	bool Initialize(void) final override;
	void Update(const uint32_t currentTime) final override;

	void SendPulse(void);

	virtual uint16_t GetDistance(void) final override;
	virtual bool SetThreshold(const uint16_t threshold) final override;
	virtual bool IsDetecting(void) final override;

private:
	EProximityCommands mSide;
	Gpio mGpioTrigger;
	InputCapture mGpioEcho;
	uint16_t mThreshold;
};
}
