#pragma once

#include "ComponentInterface.h"
#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "Servo.h"

namespace Component {
class ServosInterface : public ComponentInterface {
public:

	ServosInterface()  = default;
	~ServosInterface() = default;
	
	virtual Core::CoreStatus Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;

	virtual Servo &GetServo(const uint8_t servoId) = 0;
	virtual Pca9685Interface &GetPca9685(const uint8_t pca9685Id) = 0;
};
}
