#pragma once

#include "Service.h"
#include "../snr/SensorProximity.h"

class ServiceProximity : public Service
{
public:
  ServiceProximity(SensorProximity &SensorProximity);
  ~ServiceProximity() = default;

  virtual void Initialize(void) final override;
  virtual void Update(const uint32_t currentTime) final override;

private:
  SensorProximity &mSensorProximity;
};