#include "Builder.h"
#include "HardwareSerial.h"
#include "pins_arduino.h"

Builder::Builder(void)
	: mTwi(Twi::EI2cFreq::FREQ_400_KHZ)
	, mLedBoot(PIN_PB0)
	, mLedStatus(PIN_PB1)
	, mLedLeft(PIN_PB2)
	, mLedRight(PIN_PB3)
	, mBattery(PIN_A0)
	, mMpu9150(mTwi)
	, mSrf05Left(EProximityCommands::US_LEFT, PIN_A1, PIN_A2)
	, mSrf05Right(EProximityCommands::US_RIGHT, PIN_A3, PIN_A4)
	, mVl53l0x(mTwi)
	, mPca9685Left(mTwi, 0x41)
	, mPca9685Right(mTwi, 0x40)
	, mServos(mPca9685Left, mPca9685Right)
	, mLegs(mServos)
	, mBody(mLegs)
	, mClusters(mBattery, mMpu9150, mSensorProximity, mServos, mBody)
	, mCommunication(mClusters, mLedStatus)
	, mSensorProximity(mSrf05Left, mSrf05Right, mVl53l0x)
	, mServiceControl(mServos)
	, mServiceProximity(mSensorProximity)
	, mServiceOrientation(mMpu9150)
	, mServiceBattery(mBattery)
	, mServices(mServiceControl, mServiceProximity, mServiceOrientation, mServiceBattery) {
}

void Builder::Initialize (void) {
	Serial.begin(115200);
	Serial.print("<hello>");
	mTwi.Initialize();
	mServices.Initialize();
}

void Builder::Update (void) {
	unsigned long currentMillis = millis();

	mLedBoot.Toggle();
	mCommunication.Update();
	mServices.Update(currentMillis);
}
