#pragma once

#include "../Driver/Tick/Tick.h"
#include "../Driver/Twi/Twi.h"
#include "../Driver/Uart/Uart.h"
#include "../Driver/Adc/Adc.h"
#include "../Driver/InputCapture/InputCapture.h"
#include "../Cluster/General/ClusterGeneral.h"
#include "../Cluster/Battery/ClusterBattery.h"
#include "../Cluster/Body/ClusterBody.h"
#include "../Cluster/Imu/ClusterImu.h"
#include "../Cluster/Proximity/ClusterProximity.h"
#include "../Cluster/Servo/ClusterServo.h"
#include "../Cluster/Clusters/Clusters.h"
#include "../Component/Battery/Battery.h"
#include "../Component/Led/Led.h"
#include "../Component/Imu/Mpu9150.h"
#include "../Component/ServosController/Pca9685.h"
#include "../Component/Servos/Servos.h"
#include "../Component/Software/Software.h"
#include "../Component/Display/Ssd1306.h"
#include "../Component/Proximity/SensorProximity.h"
#include "../Component/Proximity/Ultrasound/Srf05.h"
#include "../Component/Proximity/Laser/Vl53l0x.h"
#include "../Component/Communication/Communication.h"
#include "../Service/Battery/ServiceBattery.h"
#include "../Service/Control/ServiceControl.h"
#include "../Service/Communication/ServiceCommunication.h"
#include "../Service/Display/ServiceDisplay.h"
#include "../Service/General/ServiceGeneral.h"
#include "../Service/Orientation/ServiceOrientation.h"
#include "../Service/Proximity/ServiceProximity.h"
#include "../Service/Services/Services.h"
#include "../Bot/Body/Body.h"
#include "../Bot/Legs/Legs.h"

namespace Builder
{
    class App : public Core::CoreInterface {
    public:
        App(void);

        ~App(void) = default;

        virtual Core::CoreStatus Initialize(void) final override;

        virtual void Update(const uint64_t currentTime) final override;

    private:
        Driver::Tick::Tick mTick;
        Driver::Uart::Uart mUart;
        Driver::Twi::Twi mTwi;
        Driver::Adc::Adc mAdc;
        Component::Led::Led mLedBoot;
        Component::Led::Led mLedStatus;
        Component::Led::Led mLedLeft;
        Component::Led::Led mLedRight;
        Driver::InputCapture::InputCapture mInputCaptureLeft;
        Driver::InputCapture::InputCapture mInputCaptureRight;
        Component::Battery::Battery mBattery;
        Component::Imu::Mpu9150 mMpu9150;
        Component::Proximity::Ultrasound::Srf05 mSrf05Left;
        Component::Proximity::Ultrasound::Srf05 mSrf05Right;
        Component::Proximity::Laser::Vl53l0x mVl53l0x;
        Component::Proximity::SensorProximity mSensorProximity;
        Component::Display::Ssd1306 mSsd1306;
        Component::ServosController::Pca9685 mPca9685Left;
        Component::ServosController::Pca9685 mPca9685Right;
        Component::Servos::Servos mServos;
        Component::Software::Software mSoftware;
        Bot::Legs::Legs mLegs;
        Bot::Body::Body mBody;

        Cluster::General::ClusterGeneral mClusterGeneral;
        Cluster::Battery::ClusterBattery mClusterBattery;
        Cluster::Body::ClusterBody mClusterBody;
        Cluster::Imu::ClusterImu mClusterImu;
        Cluster::Proximity::ClusterProximity mClusterProximity;
        Cluster::Servo::ClusterServo mClusterServo;
        Cluster::Clusters::Clusters mClusters;

        Component::Communication::Communication mCommunication;

        Service::Control::ServiceControl mServiceControl;
        Service::Communication::ServiceCommunication mServiceCommunication;
        Service::Proximity::ServiceProximity mServiceProximity;
        Service::Orientation::ServiceOrientation mServiceOrientation;
        Service::Battery::ServiceBattery mServiceBattery;
        Service::Display::ServiceDisplay mServiceDisplay;
        Service::General::ServiceGeneral mServiceGeneral;
        Service::Services::Services mServices;
    };
}
