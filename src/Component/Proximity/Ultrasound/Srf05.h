#pragma once

#include "../../../Cluster/Constants.h"
#include "../../../Driver/InputCapture/InputCaptureInterface.h"
#include "../../../Driver/Gpio/GpioInterface.h"
#include "../../../Driver/Tick/TickInterface.h"
#include "../../Led/LedInterface.h"
#include "../SensorProximityInterface.h"


namespace Component {
namespace Proximity {
namespace Ultrasound {
class Srf05 : public SensorProximityInterface {
public:
	static const uint64_t ECHO_TIMEOUT       = 30000U;
	static const uint16_t DISTANCE_THRESHOLD = 30U;

	Srf05( const Cluster::EProximityCommands side, Driver::Gpio::GpioInterface &gpioTrigger, Driver::InputCapture::InputCaptureInterface &gpioEcho, Led::LedInterface &led, Driver::Tick::TickInterface &tick );
	~Srf05() = default;

	Core::CoreStatus Initialize( void ) final override;
	void Update( const uint64_t currentTime ) final override;

	void SendPulse( void );
	virtual uint16_t GetDistance( void ) final override;
	virtual uint16_t GetThreshold( void ) final override;
	virtual Core::CoreStatus SetThreshold( const uint16_t threshold ) final override;

private:

	Cluster::EProximityCommands mSide;
	Driver::Gpio::GpioInterface &mGpioTrigger;
	Driver::InputCapture::InputCaptureInterface &mGpioEcho;
	Driver::Tick::TickInterface &mTick;
	Led::LedInterface &mLed;
	uint16_t mThreshold;
};
}
}
}
