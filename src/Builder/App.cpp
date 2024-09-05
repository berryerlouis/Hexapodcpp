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
        : mStartTime(0UL)
          , mTick()
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
          , mMpu9150(mTwi)
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
          , mClusterImu(mMpu9150)
          , mClusterProximity(mSensorProximity)
          , mClusterServo(mServos)
          , mClusters(mClusterGeneral, mClusterBattery, mClusterBody, mClusterImu, mClusterProximity, mClusterServo)
          , mCommunication(mUart, mClusters, mLedStatus)
          , mServiceControl(mServos)
          , mServiceCommunication(mCommunication, mClusters)
          , mServiceProximity(mSensorProximity)
          , mServiceOrientation(mMpu9150)
          , mServiceBattery(mBattery)
          , mServiceDisplay(mSsd1306, mBattery, mSensorProximity)
          , mServiceGeneral(mSoftware)
          , mServices(mServiceGeneral, mServiceControl, mServiceCommunication, mServiceProximity, mServiceOrientation, mServiceBattery, mServiceDisplay) {
        INIT_LOGGER(mUart);
    }

    Core::CoreStatus App::Initialize(void) {
        Core::CoreStatus success = mUart.Initialize();
        if (success == true) {
            success = mTwi.Initialize();
        }
        if (success == true) {
            success = mServices.Initialize();
        }
        if (success == false) {
            LOG("<error>");
        }
        this->mStartTime = this->mTick.GetMs();
        return (success);
    }

    void App::Update(const uint64_t currentTime) {
        (void) currentTime;
        const uint64_t currentMillis = this->mTick.GetMs() - this->mStartTime;
        mLedBoot.Toggle();
        mServices.Update(currentMillis);
    }
}
