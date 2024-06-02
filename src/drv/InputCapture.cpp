#include "InputCapture.h"
static InputCapture *inputCapture[2U]  = {};
static uint8_t       inputCaptureIndex = 0;

InputCapture::InputCapture(const uint8_t gpio)
	: mIo(gpio)
	, mState(false)
	, mStartTime(0)
	, mDelay(0) {
}

void InputCapture::Initialize (void) {
	pinMode(this->mIo, INPUT_PULLUP);
	PCICR  |= (1 << PCIE0);
	PCMSK0 |= (1 << (this->mIo - 24U));
	inputCapture[inputCaptureIndex] = this;
	inputCaptureIndex++;
}

void InputCapture::Update (const uint32_t currentTime) {
}

uint64_t InputCapture::GetInputCaptureTime (void) {
	return(this->mDelay);
}

void InputCapture::EdgeChange (void) {
	int state = digitalRead(this->mIo);

	if (state != this->mState && state == true) {
		this->mStartTime = micros();
	}
	else if (state != this->mState && state == false) {
		this->mDelay = micros() - this->mStartTime;
	}
	this->mState = state;
}

ISR(PCINT0_vect) {
	uint8_t oldSREG = SREG;

	cli();

	for (size_t i = 0; i < inputCaptureIndex; i++) {
		inputCapture[i]->EdgeChange();
	}

	SREG = oldSREG;
}
