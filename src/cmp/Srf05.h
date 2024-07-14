#pragma once

#include "../clu/Constants.h"
#include "../cmp/ProximityInterface.h"
#include "../drv/InputCaptureInterface.h"
#include "../drv/GpioInterface.h"
#include "../cmp/LedInterface.h"
#include "../drv/TickInterface.h"

namespace Component {
using namespace Driver;
using namespace Clusters;
class Srf05 : public ProximityInterface {
public:
	static const uint64_t ECHO_TIMEOUT       = 30000U;
	static const uint16_t DISTANCE_THRESHOLD = 30U;

	Srf05( const EProximityCommands side, GpioInterface &gpioTrigger, InputCaptureInterface &gpioEcho, LedInterface &led, TickInterface &tick );
	~Srf05() = default;

	Core::CoreStatus Initialize( void ) final override;
	void Update( const uint64_t currentTime ) final override;

	void SendPulse( void );
	virtual uint16_t GetDistance( void ) final override;
	virtual uint16_t GetThreshold( void ) final override;
	virtual Core::CoreStatus SetThreshold( const uint16_t threshold ) final override;
	virtual bool IsDetecting( void ) final override;

private:
	EProximityCommands mSide;
	GpioInterface &mGpioTrigger;
	InputCaptureInterface &mGpioEcho;
	TickInterface &mTick;
	LedInterface &mLed;
	uint16_t mThreshold;
};
}
