#include "App.h"

namespace App
{
    using Driver::Gpio::Gpio;
    using Driver::Timer::Tick;

    Gpio ledStatus = Gpio({19}, Driver::Gpio::EPortDirection::PWM);
    Gpio ledCenter = Gpio({12}, Driver::Gpio::EPortDirection::OUT);
    Gpio ledLeft = Gpio({13}, Driver::Gpio::EPortDirection::OUT);
    Gpio ledMiddleLeft = Gpio({6}, Driver::Gpio::EPortDirection::OUT);
    Gpio ledRight = Gpio({16}, Driver::Gpio::EPortDirection::OUT);
    Gpio ledMiddleRight = Gpio({5}, Driver::Gpio::EPortDirection::OUT);
    Gpio enablePwm = Gpio({17}, Driver::Gpio::EPortDirection::OUT);
    Gpio echoLeftPin = Gpio({8}, Driver::Gpio::EPortDirection::IN);
    Gpio echoRightPin = Gpio({20}, Driver::Gpio::EPortDirection::IN);
    Gpio triggerLeftPin = Gpio({7}, Driver::Gpio::EPortDirection::OUT);
    Gpio triggerRightPin = Gpio({21}, Driver::Gpio::EPortDirection::OUT);
    Gpio soundLeftPin = Gpio({23}, Driver::Gpio::EPortDirection::IN);
    Gpio soundRightPin = Gpio({24}, Driver::Gpio::EPortDirection::IN);
    Gpio buttonPin = Gpio({4}, Driver::Gpio::EPortDirection::IN);

    App::App()
        : mSocket()
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
        , mButton(mGpioButton)
        , mSoundLeft(Component::Sound::SoundId::SOUND_LEFT, mGpioSoundLeft, mLedMiddleLeft)
        , mSoundRight(Component::Sound::SoundId::SOUND_RIGHT, mGpioSoundRight, mLedMiddleRight)
        , mInputCaptureLeft(echoLeftPin)
        , mInputCaptureRight(echoRightPin)
        , mMpu9150(mTwi)
        , mBarometer(mTwi)
        , mSrf05Left(Cluster::EProximityCommands::US_LEFT,
                     mGpioTriggerUsLeft,
                     mInputCaptureLeft,
                     mLedLeft)
        , mSrf05Right(Cluster::EProximityCommands::US_RIGHT,
                      mGpioTriggerUsRight,
                      mInputCaptureRight,
                      mLedRight)
        , mVl53l5x(mTwi, mLedCenter)
        , mSensorProximity(mSrf05Left, mSrf05Right, mVl53l5x)
        , mSsd1306(mTwi)
        , mPca9685Left(mTwi, 0x41U)
        , mPca9685Right(mTwi, 0x40U)
        , mServos(mPca9685Left, mPca9685Right, mEnablePwm)
        , mSoftware()
        , mLegs(mServos)
        , mBody(mLegs)
        , mClusterGeneral(mSoftware)
        , mClusterBattery(mBattery)
        , mClusterButton(mButton)
        , mClusterSound(mSoundLeft, mSoundRight)
        , mClusterBody(mBody)
        , mClusterImu(mMpu9150, mBarometer)
        , mClusterProximity(mSensorProximity)
        , mClusterServo(mServos)
        , mClusters(mClusterGeneral,
                    mClusterBattery,
                    mClusterButton,
                    mClusterSound,
                    mClusterBody,
                    mClusterImu,
                    mClusterProximity,
                    mClusterServo)
        , mCommunication(mSocket, mLedStatus)
        , mMessageListener(mCommunication)
        , mEventDispatcher()
        , mServiceButton(mButton, mMessageListener, mEventDispatcher)
        , mServiceSound(mSoundLeft, mSoundRight, mMessageListener, mEventDispatcher)
        , mServiceControl(mServos, mMessageListener, mEventDispatcher)
        , mServiceCommunication(mCommunication, mClusters, mMessageListener, mEventDispatcher)
        , mServiceProximity(mSensorProximity, mMessageListener, mEventDispatcher)
        , mServiceOrientation(mMpu9150, mBarometer, mMessageListener, mEventDispatcher)
        , mServiceBattery(mBattery, mMessageListener, mEventDispatcher)
        , mServiceBody(mBody, mMessageListener, mEventDispatcher)
        , mServiceDisplay(mSsd1306, mMessageListener, mEventDispatcher)
        , mServiceGeneral(mLedPwmStatus, mSoftware, mMessageListener, mEventDispatcher)
        , mServices(mServiceGeneral,
                    mServiceControl,
                    mServiceCommunication,
                    mServiceProximity,
                    mServiceOrientation,
                    mServiceBattery,
                    mServiceDisplay,
                    mServiceBody,
                    mServiceButton,
                    mServiceSound,
                    mMessageListener)
        , mNextUpdateDeadlineMs(0U) {
    }

#define LOG_RESULT_INIT(name)             \
    if (success != Core::Status::CORE_OK) \
        LOG_ERROR("Initialization %s failed.", name);
#define INIT(name, code) \
    success = code;      \
    LOG_RESULT_INIT(name);

    Core::Status App::Initialize() {
        Tick::GetInstance();
        Core::Status success = Core::CORE_OK;
        INIT("Socket", this->mSocket.Initialize());
        if (success == Core::CORE_OK) {
            INIT("Twi", this->mTwi.Initialize());
        }
        if (success == Core::CORE_OK) {
            INIT("Services", this->mServices.Initialize());
        }
        return success;
    }

    void App::Update() {
        Tick          &tick = Tick::GetInstance();
        const uint64_t currentTime = tick.GetMs();

        if (this->mNextUpdateDeadlineMs == 0U) {
            this->mNextUpdateDeadlineMs = currentTime + 1U;
        }

        this->mServices.Update(currentTime);

        this->mNextUpdateDeadlineMs += 1U;
        const uint64_t afterUpdateTime = tick.GetMs();

        if (afterUpdateTime < this->mNextUpdateDeadlineMs) {
            tick.DelayMs(this->mNextUpdateDeadlineMs - afterUpdateTime);
        } else {
            // Catch up when work took longer than one period.
            this->mNextUpdateDeadlineMs = afterUpdateTime + 1U;
        }
    }
} // namespace App
