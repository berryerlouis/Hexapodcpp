#include "InputCapture.h"
#include "../cor/Isr.h"

namespace Driver {
static InputCapture *inputCapture[2U]  = {};
static uint8_t       inputCaptureIndex = 0U;

InputCapture::InputCapture( GpioInterface &gpio, TickInterface &tick )
	: mGpio( gpio )
	, mTick( tick )
	, mState( false )
	, mStartTime( 0UL )
	, mDelay( 0UL )
{
}

Core::CoreStatus InputCapture::Initialize ( void )
{
	PCICR  |= _BV( PCIE0 );
	PCMSK0 |= _BV( ( this->mGpio.GetPin() ) );
	inputCapture[inputCaptureIndex] = this;
	inputCaptureIndex++;

	return ( Core::CoreStatus::CORE_OK );
}

void InputCapture::Update ( const uint64_t currentTime )
{
	(void) currentTime;
}

uint64_t InputCapture::GetInputCaptureTime ( void )
{
	return ( this->mDelay );
}

void InputCapture::EdgeChange ( void )
{
	int state = this->mGpio.Get();

	if ( state != this->mState && state == true )
	{
		this->mStartTime = this->mTick.GetUs();
	}
	else if ( state != this->mState && state == false )
	{
		this->mDelay = this->mTick.GetUs() - this->mStartTime;
	}
	this->mState = state;
}

ISR( PCINT0_vect )
{
	ISR_EMBEDDED_CODE(
		for ( size_t i = 0U; i < inputCaptureIndex; i++ )
		{
			inputCapture[i]->EdgeChange();
		}
		);
}
}
