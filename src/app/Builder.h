#pragma once
#include <Arduino.h>
#include "../drv/Communication.h"
#include "../drv/Twi.h"
#include "../cmp/Led.h"
#include "../cmp/Battery.h"
#include "../cmp/Srf05.h"
#include "../cmp/Servos.h"
#include "../cmp/Pca9685.h"
#include "../cmp/Vl53l0x.h"
#include "../cmp/Mpu9150.h"
#include "../clu/Clusters.h"
#include "../bot/Legs.h"
#include "../bot/Body.h"
#include "../snr/SensorProximity.h"
#include "../srv/ServiceControl.h"
#include "../srv/ServiceProximity.h"
#include "../srv/ServiceOrientation.h"
#include "../srv/ServiceBattery.h"
#include "../srv/Services.h"

class Builder {

public:
  Builder(void);
  ~Builder(void) = default;

  void Initialize(void);
  void Update(void);

private:
  Twi mTwi;
  Led mLedBoot;
  Led mLedStatus;
  Led mLedLeft;
  Led mLedRight;
  Battery mBattery;
  Mpu9150 mMpu9150;
  Srf05 mSrf05Left;
  Srf05 mSrf05Right;
  Vl53l0x mVl53l0x;
  SensorProximity mSensorProximity;
  Pca9685 mPca9685Left;
  Pca9685 mPca9685Right;
  Servos mServos;
  Legs mLegs;
  Body mBody;
  Clusters mClusters;
  Communication mCommunication;

  
  ServiceControl mServiceControl;
  ServiceProximity mServiceProximity;
  ServiceOrientation mServiceOrientation;
  ServiceBattery mServiceBattery;
  Services mServices;
};