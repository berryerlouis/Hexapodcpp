#include "App.h"
#ifndef GTEST
#include "../Driver/Gpio/Gpio.h"
#else
#include "../Driver/Gpio/GpioX64.h"
#endif
#include "../Misc/Logger/Logger.h"

using namespace Driver::Gpio;

Gpio ledBoot         = Gpio( { EPort::PORT_B, EPin::PIN_0 }, EPortDirection::OUT );
Gpio ledStatus       = Gpio( { EPort::PORT_B, EPin::PIN_1 }, EPortDirection::OUT );
Gpio ledLeft         = Gpio( { EPort::PORT_B, EPin::PIN_2 }, EPortDirection::OUT );
Gpio ledRight        = Gpio( { EPort::PORT_B, EPin::PIN_3 }, EPortDirection::OUT );
Gpio adcPinBattery   = Gpio( { EPort::PORT_A, EPin::PIN_0 }, EPortDirection::IN );
Gpio echoLeftPin     = Gpio( { EPort::PORT_A, EPin::PIN_2 }, EPortDirection::IN );
Gpio echoRightPin    = Gpio( { EPort::PORT_A, EPin::PIN_4 }, EPortDirection::IN );
Gpio triggerLeftPin  = Gpio( { EPort::PORT_A, EPin::PIN_1 }, EPortDirection::OUT );
Gpio triggerRightPin = Gpio( { EPort::PORT_A, EPin::PIN_3 }, EPortDirection::OUT );

namespace Builder {
    App::App( void )
    : mTick()
    , mUart()
    , mTwi( Driver::Twi::EI2cFreq::FREQ_400_KHZ )
    , mAdc( adcPinBattery )
    , mLedBoot( ledStatus )
    , mLedStatus( ledStatus )
    , mLedLeft( ledLeft )
    , mLedRight( ledRight )
    , mInputCaptureLeft( echoLeftPin, mTick )
    , mInputCaptureRight( echoRightPin, mTick )
    , mBattery( mAdc )
    , mMpu9150( mTwi )
    , mSrf05Left( Cluster::EProximityCommands::US_LEFT, triggerLeftPin, mInputCaptureLeft, mLedLeft, mTick )
    , mSrf05Right( Cluster::EProximityCommands::US_RIGHT, triggerRightPin, mInputCaptureRight, mLedRight, mTick )
    , mVl53l0x( mTwi, mTick )
    , mSensorProximity( mSrf05Left, mSrf05Right, mVl53l0x )
    , mSsd1306( mTwi )
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
    , mServiceControl( mServos )
    , mServiceCommunication( mCommunication, mClusters )
    , mServiceProximity( mSensorProximity )
    , mServiceOrientation( mMpu9150 )
    , mServiceBattery( mBattery )
    , mServiceDisplay( mSsd1306, mBattery, mSensorProximity )
    , mServiceGeneral( mSoftware )
    , mServices( mServiceGeneral, mServiceControl, mServiceCommunication, mServiceProximity, mServiceOrientation, mServiceBattery, mServiceDisplay )
{
    INIT_LOGGER( mUart );
}

Core::CoreStatus App::Initialize ( void )
{
    Core::CoreStatus success = mUart.Initialize();
    if ( success == true )
    {
        success = mTwi.Initialize();
    }
    if ( success == true )
    {
        success = mServices.Initialize();
    }
    if ( success == false )
    {
        LOG( "<error>" );
    }
    return ( success );
}

void App::Update ( const uint64_t currentTime )
{
    (void) currentTime;
    const uint64_t currentMillis = this->mTick.GetMs();
    mLedBoot.Toggle();
    mServices.Update( currentMillis );
}
}
