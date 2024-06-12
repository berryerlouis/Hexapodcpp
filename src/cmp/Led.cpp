#include "Led.h"

namespace Component {
Led::Led(const SGpio &gpio)
	: mGpio(Gpio(gpio, EPortDirection::OUT) )
	, mState(Led::LedState::OFF)
{
}

bool Led::Initialize (void)
{
	return (this->Off() );
}

void Led::Update (const uint32_t currentTime)
{
	(void) currentTime;
}

bool Led::On ()
{
	this->mGpio.Reset();
	this->mState = ON;
	return (true);
}

bool Led::Off ()
{
	this->mGpio.Set();
	this->mState = OFF;
	return (true);
}

bool Led::Toggle ()
{
	if (ON == this->Get() )
	{
		this->Off();
	}
	else
	{
		this->On();
	}
	return (true);
}

Led::LedState Led::Get () const
{
	return (this->mState);
}
}
