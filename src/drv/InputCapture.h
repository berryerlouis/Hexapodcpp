#pragma once

#include <Arduino.h>
#include "DriverInterface.h"

class InputCapture : public DriverInterface
{
public:

  InputCapture(const uint8_t gpio);
  ~InputCapture() = default;
  
  virtual void Initialize(void) final override;
  virtual void Update(const uint32_t currentTime) final override;

  void EdgeChange(void);
  uint64_t GetInputCaptureTime(void);

private:
  const uint8_t mIo;
  bool mState;
  uint64_t mStartTime;
  uint64_t mDelay;
};