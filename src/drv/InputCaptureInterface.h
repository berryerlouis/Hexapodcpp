#pragma once

#include "DriverInterface.h"

namespace Driver {
class InputCaptureInterface : public DriverInterface
{
public:
	InputCaptureInterface()  = default;
	~InputCaptureInterface() = default;

	virtual Core::CoreStatus Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;

	virtual void EdgeChange(void) = 0;
	virtual uint64_t GetInputCaptureTime(void) = 0;
};
}
