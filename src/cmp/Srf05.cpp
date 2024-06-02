#include "Srf05.h"


Srf05::Srf05(const EProximityCommands side, const uint8_t gpioTrigger, const uint8_t gpioEcho)
	: mSide(side)
	, mGpioTrigger(gpioTrigger)
	, mGpioEcho(gpioEcho)
	, mThreshold(DISTANCE_THRESHOLD) {
}

void Srf05::Initialize (void) {
	pinMode(this->mGpioTrigger, OUTPUT);
	this->mGpioEcho.Initialize();
}

void Srf05::Update (const uint32_t currentTime) {
	this->SendPulse();
}

bool Srf05::SetThreshold (uint16_t mThreshold) {
	this->mThreshold = mThreshold;
	return(true);
}

bool Srf05::IsDetecting (void) {
	const uint16_t distance = this->GetDistance();

	return(distance != 0U && distance <= this->mThreshold);
}

void Srf05::SendPulse (void) {
	digitalWrite(this->mGpioTrigger, LOW);
	delayMicroseconds(1);
	digitalWrite(this->mGpioTrigger, HIGH);
	delayMicroseconds(10);
	digitalWrite(this->mGpioTrigger, LOW);
}

uint16_t Srf05::GetDistance (void) {
	return((uint16_t)((this->mGpioEcho.GetInputCaptureTime() / 58.0F)));
}
