#pragma once
#include "Servo.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"

static const uint8_t NB_SERVOS = 18U;
class Servos {

public:
  Servos(Pca9685 &pca9685_0, Pca9685 &pca9685_1);
  ~Servos() = default;

  void Initialize(void);
  void Update(const uint32_t currentTime);

  Servo &GetServo(const uint8_t servoId);
  Pca9685 &GetPca9685(const uint8_t pca9685Id);

  bool BuildFrameAllAngle(Frame &response);
  bool BuildFrameAngle(uint8_t servoId, Frame &response);
  bool BuildFrameMinAngle(uint8_t servoId, Frame &response);
  bool BuildFrameMaxAngle(uint8_t servoId, Frame &response);
  bool BuildFrameOffset(uint8_t servoId, Frame &response);
  bool BuildFrameState(uint8_t servoId, Frame &response);
  bool BuildFrameReverse(uint8_t servoId, Frame &response);

private:

  Servo mServo0;
  Servo mServo1;
  Servo mServo2;
  Servo mServo3;
  Servo mServo4;
  Servo mServo5;
  Servo mServo6;
  Servo mServo7;
  Servo mServo8;
  Servo mServo9;
  Servo mServo10;
  Servo mServo11;
  Servo mServo12;
  Servo mServo13;
  Servo mServo14;
  Servo mServo15;
  Servo mServo16;
  Servo mServo17;
  Servo *mServos[NB_SERVOS];
  Pca9685 &mPca9685Left;
  Pca9685 &mPca9685Right;
};
