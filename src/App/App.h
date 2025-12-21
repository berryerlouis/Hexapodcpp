#pragma once

#include "../Bot/Body/Body.h"
#include "../Bot/Legs/Legs.h"
#include "../Cluster/Button/ClusterButton.h"
#include "../Cluster/Clusters/Clusters.h"
#include "../Component/Adc/Ads1115.h"
#include "../Component/Barometer/Barometer.h"
#include "../Component/Battery/Battery.h"
#include "../Component/Button/Button.h"
#include "../Component/Communication/Communication.h"
#include "../Component/Display/Ssd1306.h"
#include "../Component/Imu/Mpu9150.h"
#include "../Component/Led/Led.h"
#include "../Component/LedPwm/LedPwm.h"
#include "../Component/Proximity/Laser/Vl53l0x.h"
#include "../Component/Proximity/SensorProximity.h"
#include "../Component/Proximity/Ultrasound/Srf05.h"
#include "../Component/Servos/Servos.h"
#include "../Component/ServosController/Pca9685.h"
#include "../Component/Software/Software.h"
#include "../Component/Sound/Sound.h"
#include "../Driver/Adc/Adc.h"
#include "../Driver/Gpio/Gpio.h"
#include "../Driver/InputCapture/InputCapture.h"
#include "../Driver/Socket/Socket.h"
#include "../Driver/Twi/Twi.h"
#include "../Driver/Uart/Uart.h"
#include "../Service/Battery/ServiceBattery.h"
#include "../Service/Body/ServiceBody.h"
#include "../Service/Button/ServiceButton.h"
#include "../Service/Communication/ServiceCommunication.h"
#include "../Service/Control/ServiceControl.h"
#include "../Service/Display/ServiceDisplay.h"
#include "../Service/Event/EventListener.h"
#include "../Service/General/ServiceGeneral.h"
#include "../Service/Message/MessageListener.h"
#include "../Service/Orientation/ServiceOrientation.h"
#include "../Service/Proximity/ServiceProximity.h"
#include "../Service/Services/Services.h"
#include "../Service/Sound/ServiceSound.h"

namespace App
{
    class App {
    public:
        App(void);

        ~App(void) = default;

        virtual Core::Status Initialize(void);

        virtual void         Update(void);

    private:
        Driver::Uart::Uart                           mUart;
        Driver::Socket::Socket                       mSocket;
        Driver::Twi::Twi                             mTwi;
        Driver::Gpio::Gpio                           mEnablePwm;
        Driver::Gpio::Gpio                           mGpioButton;
        Driver::Gpio::Gpio                           mGpioSoundLeft;
        Driver::Gpio::Gpio                           mGpioSoundRight;
        Driver::Gpio::Gpio                           mGpioTriggerUsLeft;
        Driver::Gpio::Gpio                           mGpioTriggerUsRight;
        Component::Led::Led                          mLedStatus;
        Component::Led::Led                          mLedCenter;
        Component::Led::Led                          mLedLeft;
        Component::Led::Led                          mLedMiddleLeft;
        Component::Led::Led                          mLedRight;
        Component::Led::Led                          mLedMiddleRight;
        Component::LedPwm::LedPwm                    mLedPwmStatus;
        Component::Adc::Ads1115                      mAds1115;
        Component::Battery::Battery                  mBattery;
        Component::Button::Button                    mButton;
        Component::Sound::Sound                      mSoundLeft;
        Component::Sound::Sound                      mSoundRight;
        Driver::InputCapture::InputCapture           mInputCaptureLeft;
        Driver::InputCapture::InputCapture           mInputCaptureRight;
        Component::Imu::Mpu9150                      mMpu9150;
        Component::Barometer::Barometer              mBarometer;
        Component::Proximity::Ultrasound::Srf05      mSrf05Left;
        Component::Proximity::Ultrasound::Srf05      mSrf05Right;
        Component::Proximity::Laser::Vl53l0x         mVl53l0x;
        Component::Proximity::SensorProximity        mSensorProximity;
        Component::Display::Ssd1306                  mSsd1306;
        Component::ServosController::Pca9685         mPca9685Left;
        Component::ServosController::Pca9685         mPca9685Right;
        Component::Servos::Servos                    mServos;
        Component::Software::Software                mSoftware;
        Bot::Legs::Legs                              mLegs;
        Bot::Body::Body                              mBody;

        Cluster::General::ClusterGeneral             mClusterGeneral;
        Cluster::Battery::ClusterBattery             mClusterBattery;
        Cluster::Button::ClusterButton               mClusterButton;
        Cluster::Sound::ClusterSound                 mClusterSound;
        Cluster::Body::ClusterBody                   mClusterBody;
        Cluster::Imu::ClusterImu                     mClusterImu;
        Cluster::Proximity::ClusterProximity         mClusterProximity;
        Cluster::Servo::ClusterServo                 mClusterServo;
        Cluster::Clusters::Clusters                  mClusters;

        Component::Communication::Communication      mCommunication;

        Service::Message::MessageListener            mMessageListener;
        Service::Event::EventListener                mEventListener;
        Service::Button::ServiceButton               mServiceButton;
        Service::Sound::ServiceSound                 mServiceSound;
        Service::Control::ServiceControl             mServiceControl;
        Service::Communication::ServiceCommunication mServiceCommunication;
        Service::Proximity::ServiceProximity         mServiceProximity;
        Service::Orientation::ServiceOrientation     mServiceOrientation;
        Service::Battery::ServiceBattery             mServiceBattery;
        Service::Body::ServiceBody                   mServiceBody;
        Service::Display::ServiceDisplay             mServiceDisplay;
        Service::General::ServiceGeneral             mServiceGeneral;
        Service::Services::Services                  mServices;
    };
} // namespace App
