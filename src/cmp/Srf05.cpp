#include "Srf05.h"

namespace Component {
Srf05::Srf05( const EProximityCommands side, GpioInterface &gpioTrigger, InputCaptureInterface &gpioEcho, TickInterface &tick )
	: mSide( side )
	, mGpioTrigger( gpioTrigger )
	, mGpioEcho( gpioEcho )
	, mTick( tick )
	, mThreshold( DISTANCE_THRESHOLD )
{
}

Core::CoreStatus Srf05::Initialize ( void )
{
	this->mGpioEcho.Initialize();
	return ( Core::CoreStatus::CORE_OK );
}

void Srf05::Update ( const uint64_t currentTime )
{
	(void) currentTime;
	this->SendPulse();
}

uint16_t Srf05::GetThreshold ( void )
{
	return ( this->mThreshold );
}

Core::CoreStatus Srf05::SetThreshold ( uint16_t mThreshold )
{
	this->mThreshold = mThreshold;
	return ( Core::CoreStatus::CORE_OK );
}

bool Srf05::IsDetecting ( void )
{
	const uint16_t distance = this->GetDistance();

	return ( distance != 0U && distance <= this->mThreshold );
}

void Srf05::SendPulse ( void )
{
	this->mGpioTrigger.Set();
	this->mTick.DelayUs( 10U );
	this->mGpioTrigger.Reset();
}

uint16_t Srf05::GetDistance ( void )
{
	return ( (uint16_t) ( ( this->mGpioEcho.GetInputCaptureTime() / 58.0F ) ) );
}
}
