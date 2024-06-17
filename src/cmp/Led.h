#pragma once

#include "LedInterface.h"
#include "../drv/Gpio.h"
namespace Component {
using namespace Driver;
class Led : public LedInterface {
public:

	Led( GpioInterface &gpio );
	~Led() = default;

	virtual Core::CoreStatus  Initialize( void ) final override;
	virtual void Update( const uint32_t currentTime ) final override;

	virtual Core::CoreStatus On() final override;
	virtual Core::CoreStatus Off() final override;
	virtual Core::CoreStatus Toggle() final override;
	virtual LedState Get() const final override;

private:
	GpioInterface &mGpio;
	LedState mState;
};
}
