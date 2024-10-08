#include "App.h"
#include "../Driver/Gpio/Gpio.h"
#include "../Misc/Logger/Logger.h"

using namespace Driver::Gpio;

Gpio ledBoot = Gpio({PORT_B, PIN_0}, OUT);
Gpio ledStatus = Gpio({PORT_B, PIN_1}, OUT);
Gpio ledLeft = Gpio({PORT_B, PIN_2}, OUT);
Gpio ledRight = Gpio({PORT_B, PIN_3}, OUT);
Gpio adcPinBattery = Gpio({PORT_A, PIN_0}, IN);
Gpio echoLeftPin = Gpio({PORT_A, PIN_2}, IN);
Gpio echoRightPin = Gpio({PORT_A, PIN_4}, IN);
Gpio triggerLeftPin = Gpio({PORT_A, PIN_1}, OUT);
Gpio triggerRightPin = Gpio({PORT_A, PIN_3}, OUT);


namespace Builder
{
    App::App(void)
        : mTick()
          , mUart()
          , mTwi(Driver::Twi::EI2cFreq::FREQ_400_KHZ)
          , mAdc(adcPinBattery)
          , mLedBoot(ledBoot)
          , mLedStatus(ledStatus)
          , mLedLeft(ledLeft)
          , mLedRight(ledRight)
          , mInputCaptureLeft(echoLeftPin, mTick)
          , mInputCaptureRight(echoRightPin, mTick)
          , mBattery(mAdc)
          , mMpu9150(mTwi, mTick)
          , mBarometer(mTwi)
          , mSrf05Left(Cluster::EProximityCommands::US_LEFT, triggerLeftPin, mInputCaptureLeft, mLedLeft, mTick)
          , mSrf05Right(Cluster::EProximityCommands::US_RIGHT, triggerRightPin, mInputCaptureRight, mLedRight, mTick)
          , mVl53l0x(mTwi, mTick)
          , mSensorProximity(mSrf05Left, mSrf05Right, mVl53l0x)
          , mSsd1306(mTwi)
          , mPca9685Left(mTwi, 0x41U)
          , mPca9685Right(mTwi, 0x40U)
          , mServos(mPca9685Left, mPca9685Right, mTick)
          , mSoftware()
          , mLegs(mServos)
          , mBody(mLegs)
          , mClusterGeneral(mSoftware)
          , mClusterBattery(mBattery)
          , mClusterBody(mBody)
          , mClusterImu(mMpu9150, mBarometer)
          , mClusterProximity(mSensorProximity)
          , mClusterServo(mServos)
          , mClusters(mClusterGeneral, mClusterBattery, mClusterBody, mClusterImu, mClusterProximity, mClusterServo)
          , mCommunication(mUart, mClusters, mLedStatus)
          , mEventListener()
          , mServiceControl(mServos, mEventListener)
          , mServiceCommunication(mCommunication, mClusters, mEventListener)
          , mServiceProximity(mSensorProximity, mEventListener)
          , mServiceOrientation(mMpu9150, mBarometer, mEventListener)
          , mServiceBattery(mBattery, mEventListener)
          , mServiceBody(mBody, mEventListener)
          , mServiceDisplay(mSsd1306, mEventListener)
          , mServiceGeneral(mSoftware, mEventListener)
          , mServices(mTick, mServiceGeneral, mServiceControl, mServiceCommunication, mServiceProximity,
                      mServiceOrientation, mServiceBattery, mServiceDisplay, mServiceBody, mEventListener) {
        INIT_LOGGER(mUart);
    }


    Core::CoreStatus App::Initialize(void) {
        Core::CoreStatus success = mUart.Initialize();
        if (success == Core::CoreStatus::CORE_OK) {
            success = mTwi.Initialize();
        }
        if (success == Core::CoreStatus::CORE_OK) {
            success = mServices.Initialize();
        }
        if (success != Core::CoreStatus::CORE_OK) {
            LOG("<error>");
        }
        return (success);
    }

    void App::Update(const uint64_t currentTime) {
        mLedBoot.Toggle();
        mServices.Update(mTick.GetMs());
    }
}
