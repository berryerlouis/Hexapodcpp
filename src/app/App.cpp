#include "App.h"
#include <avr/wdt.h>

Gpio ledBoot         = Gpio({ EPort::PORT_B, EPin::PIN_0 }, EPortDirection::OUT);
Gpio ledStatus       = Gpio({ EPort::PORT_B, EPin::PIN_1 }, EPortDirection::OUT);
Gpio ledLeft         = Gpio({ EPort::PORT_B, EPin::PIN_2 }, EPortDirection::OUT);
Gpio ledRight        = Gpio({ EPort::PORT_B, EPin::PIN_3 }, EPortDirection::OUT);
Gpio adcPinBattery   = Gpio({ EPort::PORT_A, EPin::PIN_0 }, EPortDirection::IN);
Gpio echoLeftPin     = Gpio({ EPort::PORT_A, EPin::PIN_2 }, EPortDirection::IN);
Gpio echoRightPin    = Gpio({ EPort::PORT_A, EPin::PIN_4 }, EPortDirection::IN);
Gpio triggerLeftPin  = Gpio({ EPort::PORT_A, EPin::PIN_1 }, EPortDirection::OUT);
Gpio triggerRightPin = Gpio({ EPort::PORT_A, EPin::PIN_3 }, EPortDirection::OUT);

namespace app {
App::App(void)
	: mTick()
	, mUart()
	, mTwi(Twi::EI2cFreq::FREQ_400_KHZ)
	, mAdc(adcPinBattery)
	, mLedBoot(ledStatus)
	, mLedStatus(ledStatus)
	, mLedLeft(ledLeft)
	, mLedRight(ledRight)
	, mICLeft(echoLeftPin, mTick)
	, mICRight(echoRightPin, mTick)
	, mBattery(mAdc)
	, mMpu9150(mTwi)
	, mSrf05Left(EProximityCommands::US_LEFT, triggerLeftPin, mICLeft, mTick)
	, mSrf05Right(EProximityCommands::US_RIGHT, triggerRightPin, mICRight, mTick)
	, mVl53l0x(mTwi, mTick)
	, mSensorProximity(mSrf05Left, mSrf05Right, mVl53l0x)
	, mPca9685Left(mTwi, 0x41)
	, mPca9685Right(mTwi, 0x40)
	, mServos(mPca9685Left, mPca9685Right, mTick)
	, mLegs(mServos)
	, mBody(mLegs)
	, mClusters(mBattery, mMpu9150, mSensorProximity, mServos, mBody)
	, mCommunication(mUart, mClusters, mLedStatus)
	, mServiceControl(mServos)
	, mServiceProximity(mSensorProximity)
	, mServiceOrientation(mMpu9150)
	, mServiceBattery(mBattery)
	, mServices(mCommunication, mServiceControl, mServiceProximity, mServiceOrientation, mServiceBattery)
{
}

bool App::Initialize (void)
{
	bool success = false;
	success = mUart.Initialize();
	if (success == true)
	{
		success = mTwi.Initialize();
	}
	if (success == true)
	{
		success = mServices.Initialize();
	}
	if (success == true)
	{
		mUart.Send("<hello>", strlen("<hello>") );
	}
	else
	{
		mUart.Send("<error>", strlen("<error>") );
	}
	wdt_enable(WDTO_15MS);
	return (success);
}

void App::Update (void)
{
	wdt_reset();

	unsigned long currentMillis = this->mTick.GetMs();
	mLedBoot.Toggle();
	mCommunication.Update(currentMillis);
	mServices.Update(currentMillis);
}
}
