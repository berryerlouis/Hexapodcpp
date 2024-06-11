#pragma once

#include "DriverInterface.h"
#include "Gpio.h"

class InputCapture : public DriverInterface {
public:
	InputCapture(const SGpio &gpio);
	~InputCapture() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	void EdgeChange(void);
	uint64_t GetInputCaptureTime(void);

private:
	Gpio mGpio;
	bool mState;
	uint64_t mStartTime;
	uint64_t mDelay;
};
