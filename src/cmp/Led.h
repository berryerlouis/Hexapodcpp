#pragma once

#include "../drv/Gpio.h"

class Led {
public:
	enum LedState
	{
		ON = 0,
		OFF
	};

	Led(const SGpio &gpio);
	~Led() = default;


	bool On();
	bool Off();
	bool Toggle();
	LedState Get() const;

private:
	Gpio mGpio;
	LedState mState;
};
