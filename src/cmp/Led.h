#pragma once

#include "ComponentInterface.h"
#include "../drv/Gpio.h"
namespace Component {
using namespace Driver;
class Led : public ComponentInterface {
public:
	enum LedState
	{
		ON = 0,
		OFF
	};

	Led(GpioInterface &gpio);
	~Led() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	bool On();
	bool Off();
	bool Toggle();
	LedState Get() const;

private:
	GpioInterface &mGpio;
	LedState mState;
};
}
