#include "App.h"

Gpio ledBoot         = Gpio( { EPort::PORT_B, EPin::PIN_0 }, EPortDirection::OUT );
Gpio ledStatus       = Gpio( { EPort::PORT_B, EPin::PIN_1 }, EPortDirection::OUT );
Gpio ledLeft         = Gpio( { EPort::PORT_B, EPin::PIN_2 }, EPortDirection::OUT );
Gpio ledRight        = Gpio( { EPort::PORT_B, EPin::PIN_3 }, EPortDirection::OUT );
Gpio adcPinBattery   = Gpio( { EPort::PORT_A, EPin::PIN_0 }, EPortDirection::IN );
Gpio echoLeftPin     = Gpio( { EPort::PORT_A, EPin::PIN_2 }, EPortDirection::IN );
Gpio echoRightPin    = Gpio( { EPort::PORT_A, EPin::PIN_4 }, EPortDirection::IN );
Gpio triggerLeftPin  = Gpio( { EPort::PORT_A, EPin::PIN_1 }, EPortDirection::OUT );
Gpio triggerRightPin = Gpio( { EPort::PORT_A, EPin::PIN_3 }, EPortDirection::OUT );

namespace app {
App::App( void )
	: mTick()
	, mUart()
	, mTwi( Twi::EI2cFreq::FREQ_400_KHZ )
	, mAdc( adcPinBattery )
	, mLedBoot( ledStatus )
	, mLedStatus( ledStatus )
	, mLedLeft( ledLeft )
	, mLedRight( ledRight )
	, mICLeft( echoLeftPin, mTick )
	, mICRight( echoRightPin, mTick )
	, mBattery( mAdc )
	, mMpu9150( mTwi )
	, mSrf05Left( EProximityCommands::US_LEFT, triggerLeftPin, mICLeft, mTick )
	, mSrf05Right( EProximityCommands::US_RIGHT, triggerRightPin, mICRight, mTick )
	, mVl53l0x( mTwi, mTick )
	, mSensorProximity( mSrf05Left, mSrf05Right, mVl53l0x )
	, mPca9685Left( mTwi, 0x41U )
	, mPca9685Right( mTwi, 0x40U )
	, mServos( mPca9685Left, mPca9685Right, mTick )
	, mSoftware()
	, mLegs( mServos )
	, mBody( mLegs )
	, mClusterGeneral( mSoftware )
	, mClusterBattery( mBattery )
	, mClusterBody( mBody )
	, mClusterImu( mMpu9150 )
	, mClusterProximity( mSensorProximity )
	, mClusterServo( mServos )
	, mClusters( mClusterGeneral, mClusterBattery, mClusterBody, mClusterImu, mClusterProximity, mClusterServo )
	, mCommunication( mUart, mClusters, mLedStatus )
	, mServiceControl( mClusterServo )
	, mServiceProximity( mClusterProximity )
	, mServiceOrientation( mClusterImu )
	, mServiceBattery( mClusterBattery )
	, mServiceGeneral( mClusterGeneral )
	, mServices( mCommunication, mServiceGeneral, mServiceControl, mServiceProximity, mServiceOrientation, mServiceBattery )
{
}

Core::CoreStatus App::Initialize ( void )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	success = mUart.Initialize();
	if ( success == true )
	{
		success = mTwi.Initialize();
	}
	if ( success == true )
	{
		success = mServices.Initialize();
	}
	if ( success == true )
	{
		mUart.Send( "<hello>", strlen( "<hello>" ) );
	}
	else
	{
		mUart.Send( "<error>", strlen( "<error>" ) );
	}
	return ( success );
}

void App::Update ( const uint32_t currentTime )
{
	(void) currentTime;
	unsigned long currentMillis = this->mTick.GetMs();
	mLedBoot.Toggle();
	mServices.Update( currentMillis );
}
}
