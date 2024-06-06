#include "Isr.h"
#include "InputCapture.h"
#include "Tick.h"

static InputCapture *inputCapture[2U]  = {};
static uint8_t       inputCaptureIndex = 0U;

InputCapture::InputCapture(const SGpio &gpio)
	: mGpio(Gpio(gpio, EPortDirection::IN) )
	, mState(false)
	, mStartTime(0UL)
	, mDelay(0UL)
{
}

void InputCapture::Initialize (void)
{
	PCICR  |= (1U << PCIE0);
	PCMSK0 |= (1U << (this->mGpio.GetPin() ) );
	inputCapture[inputCaptureIndex] = this;
	inputCaptureIndex++;
}

void InputCapture::Update (const uint32_t currentTime)
{
	(void) currentTime;
}

uint64_t InputCapture::GetInputCaptureTime (void)
{
	return (this->mDelay);
}

void InputCapture::EdgeChange (void)
{
	int state = this->mGpio.Get();

	if (state != this->mState && state == true)
	{
		this->mStartTime = MyTick.GetUs();
	}
	else if (state != this->mState && state == false)
	{
		this->mDelay = MyTick.GetUs() - this->mStartTime;
	}
	this->mState = state;
}

ISR(PCINT0_vect)
{
	ISR_EMBEDDED_CODE(
		for ( size_t i = 0U; i < inputCaptureIndex; i++ )
	{
		inputCapture[i]->EdgeChange();
	}
		);
}
