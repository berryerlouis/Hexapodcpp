#pragma once

#include "SensorInterface.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"

class SensorProximityInterface : public SensorInterface
{
public:
  enum SensorsId
  {
    SRF_LEFT = 0x00U,
    SRF_RIGHT,
    VLX
  };
  SensorProximityInterface(void) = default;
  ~SensorProximityInterface() = default;

  virtual uint16_t GetDistance( const SensorsId sensorId ) = 0;
  virtual bool SetThreshold( const SensorsId sensorId, const uint16_t threshold) = 0;
  virtual bool IsDetecting( const SensorsId sensorId ) = 0;

private:
};