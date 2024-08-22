#pragma once

#include "InputCaptureInterface.h"
#include "../Tick/TickInterface.h"
#include "../Gpio/GpioInterface.h"

namespace Driver {
namespace InputCapture {
class InputCapture : public InputCaptureInterface {
public:
	InputCapture( Gpio::GpioInterface &gpio, Tick::TickInterface &tick );
	~InputCapture() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void EdgeChange( void ) final override;
	virtual uint64_t GetInputCaptureTime( void ) final override;

private:
	Gpio::GpioInterface &mGpio;
	Tick::TickInterface &mTick;
	bool mState;
	uint64_t mStartTime;
	uint64_t mDelay;
};
}
}
