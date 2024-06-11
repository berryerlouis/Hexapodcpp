#include "App.h"
#include "../drv/Tick.h"

App::App(void)
	: mTwi(Twi::EI2cFreq::FREQ_400_KHZ)
	, mLedBoot({ EPort::PORT_B, EPin::PIN_0 })
	, mLedStatus({ EPort::PORT_B, EPin::PIN_1 })
	, mLedLeft({ EPort::PORT_B, EPin::PIN_2 })
	, mLedRight({ EPort::PORT_B, EPin::PIN_3 })
	, mBattery({ EPort::PORT_A, EPin::PIN_0 })
	, mMpu9150(mTwi)
	, mSrf05Left(EProximityCommands::US_LEFT, { EPort::PORT_A, EPin::PIN_1 }, { EPort::PORT_A, EPin::PIN_2 })
	, mSrf05Right(EProximityCommands::US_RIGHT, { EPort::PORT_A, EPin::PIN_3 }, { EPort::PORT_A, EPin::PIN_4 })
	, mVl53l0x(mTwi)
	, mSensorProximity(mSrf05Left, mSrf05Right, mVl53l0x)
	, mPca9685Left(mTwi, 0x41)
	, mPca9685Right(mTwi, 0x40)
	, mServos(mPca9685Left, mPca9685Right)
	, mLegs(mServos)
	, mBody(mLegs)
	, mClusters(mBattery, mMpu9150, mSensorProximity, mServos, mBody)
	, mCommunication(mClusters, mLedStatus)
	, mServiceControl(mServos)
	, mServiceProximity(mSensorProximity)
	, mServiceOrientation(mMpu9150)
	, mServiceBattery(mBattery)
	, mServices(mServiceControl, mServiceProximity, mServiceOrientation, mServiceBattery)
{
}

bool App::Initialize (void)
{
	bool success = false;
	success = MySerial.Initialize();
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
		MySerial.Send("<hello>", strlen("<hello>") );
	}
	else
	{
		MySerial.Send("<error>", strlen("<error>") );
	}
	return (success);
}

void App::Update (void)
{
	unsigned long currentMillis = MyTick.GetMs();

	mLedBoot.Toggle();
	mCommunication.Update();
	mServices.Update(currentMillis);
}
