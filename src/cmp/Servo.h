#pragma once
#include <Arduino.h>
#include "Pca9685.h"

class Servo {
  static const uint16_t SERVO_PWM_MIN = 168U;  // 0,0009 / ((1/50)/4096)
  static const uint16_t SERVO_PWM_MAX = 413U;  // 0,00220 / ((1/50)/4096)
  static const uint16_t SERVO_ANGLE_MIN = 0;
  static const uint16_t SERVO_ANGLE_MAX = 180;
  static const uint16_t SERVO_ANGLE_RANGE = 180;

public:
  Servo(Pca9685 &pca9685, const uint8_t servoId);
  Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle);
  Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle, const int8_t offset);
  Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max);
  Servo(Pca9685 &pca9685, const uint8_t servoId, const uint8_t angle, const int8_t offset, const uint8_t min, const uint8_t max, const bool reverse);
  ~Servo() = default;

  void Initialize(void);

  bool SetAngle(const uint8_t angle, const uint16_t travelTime = 0);
  uint8_t GetAngle(void) const;
  bool SetMin(const uint8_t angle);
  uint8_t GetMin(void) const;
  bool SetMax(const uint8_t angle);
  uint8_t GetMax(void) const;
  bool SetOffset(const int8_t angle);
  int8_t GetOffset(void) const;
  void SetReverse(const bool reverse);
  bool GetReverse(void);
  void SetEnable(const bool enable);
  bool IsEnable(void);
  bool IsMoving(void);

  void Update(const uint64_t currentTime);

private:
  uint8_t GetAngleFromDeltaTime(const uint64_t currentTime);
  uint8_t Lerp(uint8_t a, uint8_t b, float t);
  
  Pca9685 &mPca9685;
  uint8_t mServoId;
  uint8_t mAngle;
  uint8_t mTargetAngle;
  uint64_t mStartTime;
  uint16_t mSpeed;
  int8_t mOffset;
  uint8_t mMin;
  uint8_t mMax;
  bool mReverse;
  bool mEnable;
  bool mIsMoving;
};