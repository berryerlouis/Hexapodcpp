#include "App.h"

using namespace Driver::Gpio;

Gpio ledStatus = Gpio({19}, OUT);
Gpio ledCenter = Gpio({12}, OUT);
Gpio ledLeft = Gpio({13}, OUT);
Gpio ledMiddleLeft = Gpio({6}, OUT);
Gpio ledRight = Gpio({16}, OUT);
Gpio ledMiddleRight = Gpio({5}, OUT);
Gpio enablePwm = Gpio({17}, OUT);
Gpio echoLeftPin = Gpio({8}, IN);
Gpio echoRightPin = Gpio({20}, IN);
Gpio triggerLeftPin = Gpio({7}, OUT);
Gpio triggerRightPin = Gpio({21}, OUT);
Gpio soundLeftPin = Gpio({23}, IN);
Gpio soundRightPin = Gpio({24}, IN);
Gpio buttonPin = Gpio({4}, IN);


namespace Builder
{


    App::App(void) :
        mTick()
        , mUart()
        , mSocket()
        , mTwi(Driver::Twi::EI2cFreq::FREQ_400_KHZ)
        , mEnablePwm(enablePwm)
        , mGpioButton(buttonPin)
        , mGpioSoundLeft(soundLeftPin)
        , mGpioSoundRight(soundRightPin)
        , mGpioTriggerUsLeft(triggerLeftPin)
        , mGpioTriggerUsRight(triggerRightPin)
        , mLedStatus(ledStatus)
        , mLedCenter(ledCenter)
        , mLedLeft(ledLeft)
        , mLedMiddleLeft(ledMiddleLeft)
        , mLedRight(ledRight)
        , mLedMiddleRight(ledMiddleRight)
        , mLedPwmStatus(mLedStatus)
        , mAds1115(mTwi)
        , mBattery(mAds1115)
        , mButton(mGpioButton, mTick)
        , mSoundLeft(Component::Sound::SoundId::SOUND_LEFT, mGpioSoundLeft, mLedMiddleLeft, mTick)
        , mSoundRight(Component::Sound::SoundId::SOUND_RIGHT, mGpioSoundRight, mLedMiddleRight, mTick)
        , mInputCaptureLeft(echoLeftPin, mTick)
        , mInputCaptureRight(echoRightPin, mTick)
        , mMpu9150(mTwi, mTick)
        , mBarometer(mTwi)
        , mSrf05Left(Cluster::EProximityCommands::US_LEFT, mGpioTriggerUsLeft, mInputCaptureLeft, mLedLeft,
                     mTick)
        , mSrf05Right(Cluster::EProximityCommands::US_RIGHT, mGpioTriggerUsRight, mInputCaptureRight,
                      mLedRight, mTick)
        , mVl53l0x(mTwi, mLedCenter, mTick)
        , mSensorProximity(mSrf05Left, mSrf05Right, mVl53l0x)
        , mSsd1306(mTwi)
        , mPca9685Left(mTwi, 0x41U)
        , mPca9685Right(mTwi, 0x40U)
        , mServos(mPca9685Left, mPca9685Right, mEnablePwm, mTick)
        , mSoftware()
        , mLegs(mServos)
        , mBody(mLegs, mTick)
        , mClusterGeneral(mSoftware)
        , mClusterBattery(mBattery)
        , mClusterButton(mButton)
        , mClusterSound(mSoundLeft, mSoundRight)
        , mClusterBody(mBody)
        , mClusterImu(mMpu9150, mBarometer)
        , mClusterProximity(mSensorProximity)
        , mClusterServo(mServos)
        , mClusters(mClusterGeneral
                    , mClusterBattery
                    , mClusterButton
                    , mClusterSound
                    , mClusterBody
                    , mClusterImu
                    , mClusterProximity
                    , mClusterServo)
        , mCommunication(mSocket, mClusters, mLedStatus)
        , mMessageListener(mCommunication)
        , mServiceButton(mButton, mMessageListener)
        , mServiceSound(mSoundLeft, mSoundRight, mMessageListener)
        , mServiceControl(mServos, mMessageListener)
        , mServiceCommunication(mCommunication, mClusters, mMessageListener)
        , mServiceProximity(mSensorProximity, mMessageListener)
        , mServiceOrientation(mMpu9150, mBarometer, mMessageListener)
        , mServiceBattery(mBattery, mMessageListener)
        , mServiceBody(mBody, mMessageListener)
        , mServiceDisplay(mSsd1306, mCommunication, mBattery, mButton, mSoundLeft, mSoundRight, mSensorProximity,
                          mMessageListener, mTick)
        , mServiceGeneral(mSoftware, mMessageListener)
        , mServices(
                mTick,
                mServiceGeneral,
                mServiceControl,
                mServiceCommunication,
                mServiceProximity,
                mServiceOrientation,
                mServiceBattery,
                mServiceDisplay,
                mServiceBody,
                mServiceButton,
                mServiceSound,
                mMessageListener) {
    }

#define LOG_RESULT_INIT(name) if(success != Core::Status::CORE_OK) \
    LOG_ERROR("Initialization %s failed.",name);
#define INIT(name, code)  \
    success = code; \
    LOG_RESULT_INIT(name);

    Core::Status App::Initialize(void) {
        Core::Status success = Core::CORE_OK;
        INIT("Socket", this->mSocket.Initialize());
        if (success == Core::CORE_OK)
        INIT("Twi", this->mTwi.Initialize());
        if (success == Core::CORE_OK)
        INIT("Services", this->mServices.Initialize());
        if (success == Core::CORE_OK)
        INIT("Led", this->mLedPwmStatus.Initialize());
        return (success);
    }

    void App::Update(void) {
        const uint64_t currentTime = mTick.GetMs();
        this->mServices.Update(currentTime);
        this->mLedPwmStatus.Update(currentTime);
        this->mTick.DelayUs(100U);
    }

} // namespace Builder
