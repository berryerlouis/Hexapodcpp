#include "App.h"


#include "../Misc/Logger/Logger.h"

using namespace Driver::Gpio;

#ifdef RPI
Gpio ledStatus = Gpio({16}, OUT);
Gpio ledCenter = Gpio({12}, OUT);
Gpio ledLeft = Gpio({13}, OUT);
Gpio ledMiddleLeft = Gpio({6}, OUT);
Gpio ledRight = Gpio({19}, OUT);
Gpio ledMiddleRight = Gpio({5}, OUT);
Gpio enablePwm = Gpio({17}, OUT);
Gpio echoLeftPin = Gpio({8}, IN);
Gpio echoRightPin = Gpio({20}, IN);
Gpio triggerLeftPin = Gpio({7}, OUT);
Gpio triggerRightPin = Gpio({21}, OUT);
Gpio adcPinBattery = Gpio({0}, IN);
#else
Gpio ledBoot = Gpio({PORT_B, PIN_0}, OUT);
Gpio ledStatus = Gpio({PORT_B, PIN_1}, OUT);
Gpio ledLeft = Gpio({PORT_B, PIN_2}, OUT);
Gpio ledRight = Gpio({PORT_B, PIN_3}, OUT);
Gpio adcPinBattery = Gpio({PORT_A, PIN_0}, IN);
Gpio echoLeftPin = Gpio({PORT_A, PIN_2}, IN);
Gpio echoRightPin = Gpio({PORT_A, PIN_4}, IN);
Gpio triggerLeftPin = Gpio({PORT_A, PIN_1}, OUT);
Gpio triggerRightPin = Gpio({PORT_A, PIN_3}, OUT);
#endif

namespace Builder
{
    App::App(void) :
        mTick()
        , mUart()
        , mTwi(Driver::Twi::EI2cFreq::FREQ_400_KHZ)
#ifdef RPI
        , mAdc(adcPinBattery)
        , mLedStatus(ledStatus)
        , mLedCenter(ledCenter)
        , mLedLeft(ledLeft)
        , mLedMiddleLeft(ledMiddleLeft)
        , mLedRight(ledRight)
        , mLedMiddleRight(ledMiddleRight)
        , mEnablePwm(enablePwm)
        , mBattery(mAdc)
        , mInputCaptureLeft(echoLeftPin, mTick)
        , mInputCaptureRight(echoRightPin, mTick)
        , mMpu9150(mTwi, mTick)
        , mBarometer(mTwi)
        , mSrf05Left(Cluster::EProximityCommands::US_LEFT, triggerLeftPin, mInputCaptureLeft, mLedLeft, mTick)
        , mSrf05Right(Cluster::EProximityCommands::US_RIGHT, triggerRightPin, mInputCaptureRight, mLedRight, mTick)
        , mVl53l0x(mTwi, mLedCenter, mTick)
        , mSensorProximity(mSrf05Left, mSrf05Right, mVl53l0x)
        , mSsd1306(mTwi)
        , mPca9685Left(mTwi, 0x41U)
        , mPca9685Right(mTwi, 0x40U)
        , mServos(mPca9685Left, mPca9685Right, enablePwm, mTick)
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
                    mServiceOrientation, mServiceBattery, mServiceDisplay, mServiceBody, mEventListener)
#else
        , mAdc(adcPinBattery)
        , mLedBoot(ledBoot)
        , mLedStatus(ledStatus)
        , mLedLeft(ledLeft)
        , mLedRight(ledRight)
        , mBattery(mAdc)
        , mInputCaptureLeft(echoLeftPin, mTick)
        , mInputCaptureRight(echoRightPin, mTick)
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
                    mServiceOrientation, mServiceBattery, mServiceDisplay, mServiceBody, mEventListener)
#endif
    {
        INIT_LOGGER(mUart);
    }

    Core::Status App::Initialize(void) {
        Core::Status success = mUart.Initialize();
        if (success == Core::Status::CORE_OK) {
            success = mTwi.Initialize();
        }
        if (success == Core::Status::CORE_OK) {
            success = mServices.Initialize();
        }
        if (success != Core::Status::CORE_OK) {
            LOG("<error>");
        }
        return (success);
    }

    void App::Update(void) {
        mLedStatus.Toggle();
        const uint64_t currentTime = mTick.GetMs();
        mServices.Update(currentTime);
    }

} // namespace Builder
