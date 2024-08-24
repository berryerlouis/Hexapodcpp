#include "Led.h"

namespace Component {
namespace Led {
Led::Led( Driver::Gpio::GpioInterface &gpio )
	: mGpio( gpio )
	, mState( Led::LedState::OFF )
{
}

Core::CoreStatus Led::Initialize ( void )
{
	return ( this->Off() );
}

void Led::Update ( const uint64_t currentTime )
{
	(void) currentTime;
}

Core::CoreStatus Led::On ()
{
	this->mState = ON;
	return ( this->mGpio.Reset() );
}

Core::CoreStatus Led::Off ()
{
	this->mState = OFF;
	return ( this->mGpio.Set() );
}

Core::CoreStatus Led::Toggle ()
{
	if ( ON == this->Get() )
	{
		this->Off();
	}
	else
	{
		this->On();
	}
	return ( Core::CoreStatus::CORE_OK );
}

Led::LedState Led::Get () const
{
	return ( this->mState );
}
}
}
