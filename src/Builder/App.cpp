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
Gpio soundLeftPin = Gpio({23}, IN);
Gpio soundRightPin = Gpio({24}, IN);
Gpio adcPinBattery = Gpio({0}, IN);
Gpio buttonPin = Gpio({4}, IN);
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
        , mSocket()
        , mTwi(Driver::Twi::EI2cFreq::FREQ_400_KHZ)
        , mAdc(adcPinBattery)
        , mEnablePwm(enablePwm)
        , mGpioButton(buttonPin)
        , mGpioSoundLeft(soundLeftPin)
        , mGpioSoundRight(soundRightPin)
        , mLedStatus(ledStatus)
        , mLedCenter(ledCenter)
        , mLedLeft(ledLeft)
        , mLedMiddleLeft(ledMiddleLeft)
        , mLedRight(ledRight)
        , mLedMiddleRight(ledMiddleRight)
        , mBattery(mAdc)
        , mButton(mGpioButton, mTick)
        , mSoundLeft(Component::Sound::SoundId::SOUND_LEFT, mGpioSoundLeft, mLedMiddleLeft, mTick)
        , mSoundRight(Component::Sound::SoundId::SOUND_RIGHT, mGpioSoundRight, mLedMiddleRight, mTick)
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
        , mServos(mPca9685Left, mPca9685Right, mEnablePwm, mTick)
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
        , mServiceDisplay(mSsd1306, mButton, mSoundLeft, mSoundRight, mSensorProximity, mMessageListener)
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
        INIT_LOGGER(mSocket);
    }

    Core::Status App::Initialize(void) {
        Core::Status success = mSocket.Initialize();
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
