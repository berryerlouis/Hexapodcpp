#pragma once

#include <Arduino.h>
#include "../drv/InputCapture.h"
#include "../clu/Constants.h"
#include "../cmp/ProximityInterface.h"

class Srf05 : public ProximityInterface
{
public:
  static const uint64_t ECHO_TIMEOUT = 30000U;
  static const uint16_t DISTANCE_THRESHOLD = 30U;

  Srf05(const EProximityCommands side, const uint8_t gpioTrigger, const uint8_t gpioEcho);
  ~Srf05() = default;

  void Initialize(void);
  void Update(const uint32_t currentTime);

  void SendPulse(void);

  virtual uint16_t GetDistance( void ) final override;
  virtual bool SetThreshold( const uint16_t threshold) final override;
  virtual bool IsDetecting( void ) final override;

private:
  EProximityCommands mSide;
  uint8_t mGpioTrigger;
  InputCapture mGpioEcho;
  uint16_t mThreshold;
};