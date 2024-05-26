#pragma once

class DriverInterface {
public:
  DriverInterface(void) = default;
  ~DriverInterface() = default;

  virtual void Initialize(void) = 0;
  virtual void Update(const uint32_t currentTime) = 0;
};
