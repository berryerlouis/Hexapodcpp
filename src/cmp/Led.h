#pragma once

#include "LedInterface.h"
#include "../drv/Gpio.h"
namespace Component {
using namespace Driver;
class Led : public LedInterface {
public:

	Led(GpioInterface &gpio);
	~Led() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	virtual bool On() final override;
	virtual bool Off() final override;
	virtual bool Toggle() final override;
	virtual LedState Get() const final override;

private:
	GpioInterface &mGpio;
	LedState mState;
};
}
