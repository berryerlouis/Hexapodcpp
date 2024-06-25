#pragma once

#include "../cor/Log.h"
#include "../drv/Gpio.h"
#include "../drv/Tick.h"
#include "../drv/Twi.h"
#include "../drv/Uart.h"
#include "../drv/Adc.h"
#include "../drv/InputCapture.h"
#include "../clu/ClusterGeneral.h"
#include "../clu/ClusterBattery.h"
#include "../clu/ClusterBody.h"
#include "../clu/ClusterImu.h"
#include "../clu/ClusterProximity.h"
#include "../clu/ClusterServo.h"
#include "../clu/Clusters.h"
#include "../cmp/Battery.h"
#include "../cmp/Led.h"
#include "../cmp/Mpu9150.h"
#include "../cmp/Pca9685.h"
#include "../cmp/Servos.h"
#include "../cmp/Software.h"
#include "../cmp/Srf05.h"
#include "../cmp/Ssd1306.h"
#include "../cmp/Vl53l0x.h"
#include "../cmp/Communication.h"
#include "../cmp/SensorProximity.h"
#include "../srv/ServiceBattery.h"
#include "../srv/ServiceControl.h"
#include "../srv/ServiceDisplay.h"
#include "../srv/ServiceGeneral.h"
#include "../srv/ServiceOrientation.h"
#include "../srv/ServiceProximity.h"
#include "../srv/Services.h"
#include "../bot/Body.h"
#include "../bot/Legs.h"


using namespace Clusters;
using namespace Component;
using namespace Driver;
namespace app {
class App : public Core::CoreInterface {
public:
	App( void );
	~App( void ) = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

private:

	Tick mTick;
	Uart mUart;
	Twi mTwi;
	Adc mAdc;
	Led mLedBoot;
	Led mLedStatus;
	Led mLedLeft;
	Led mLedRight;
	InputCapture mInputCaptureLeft;
	InputCapture mInputCaptureRight;
	Battery mBattery;
	Mpu9150 mMpu9150;
	Srf05 mSrf05Left;
	Srf05 mSrf05Right;
	Ssd1306 mSsd1306;
	Vl53l0x mVl53l0x;
	SensorProximity mSensorProximity;
	Pca9685 mPca9685Left;
	Pca9685 mPca9685Right;
	Servos mServos;
	Software mSoftware;
	Legs mLegs;
	Body mBody;

	ClusterGeneral mClusterGeneral;
	ClusterBattery mClusterBattery;
	ClusterBody mClusterBody;
	ClusterImu mClusterImu;
	ClusterProximity mClusterProximity;
	ClusterServo mClusterServo;
	Clusters::Clusters mClusters;

	Communication mCommunication;

	ServiceControl mServiceControl;
	ServiceProximity mServiceProximity;
	ServiceOrientation mServiceOrientation;
	ServiceBattery mServiceBattery;
	ServiceDisplay mServiceDisplay;
	ServiceGeneral mServiceGeneral;
	Services mServices;
};
}
