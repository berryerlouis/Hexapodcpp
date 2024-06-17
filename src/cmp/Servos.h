#pragma once

#include "ServosInterface.h"

namespace Component {
using namespace Cluster;

static const uint8_t NB_SERVOS = 18U;
class Servos : public ServosInterface {
public:
	Servos(Pca9685Interface &pca9685_0, Pca9685Interface &pca9685_1, TickInterface &tick);
	~Servos() = default;

	virtual Core::CoreStatus Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	virtual Servo &GetServo(const uint8_t servoId) final override;
	virtual Pca9685Interface &GetPca9685(const uint8_t pca9685Id) final override;

	Core::CoreStatus BuildFrameAllAngle(Frame &response);
	Core::CoreStatus BuildFrameAngle(uint8_t servoId, Frame &response);
	Core::CoreStatus BuildFrameMinAngle(uint8_t servoId, Frame &response);
	Core::CoreStatus BuildFrameMaxAngle(uint8_t servoId, Frame &response);
	Core::CoreStatus BuildFrameOffset(uint8_t servoId, Frame &response);
	Core::CoreStatus BuildFrameState(uint8_t servoId, Frame &response);
	Core::CoreStatus BuildFrameReverse(uint8_t servoId, Frame &response);

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
	Pca9685Interface &mPca9685Left;
	Pca9685Interface &mPca9685Right;
};
}
