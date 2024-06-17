#pragma once

#include "InputCaptureInterface.h"
#include "TickInterface.h"
#include "GpioInterface.h"

namespace Driver {
class InputCapture : public InputCaptureInterface {
public:
	InputCapture(GpioInterface &gpio, TickInterface &tick);
	~InputCapture() = default;

	virtual Core::CoreStatus Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	virtual void EdgeChange(void) final override;
	virtual uint64_t GetInputCaptureTime(void) final override;

private:
	GpioInterface &mGpio;
	TickInterface &mTick;
	bool mState;
	uint64_t mStartTime;
	uint64_t mDelay;
};
}
