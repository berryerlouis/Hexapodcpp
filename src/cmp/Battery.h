#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../drv/Adc.h"
#include <Arduino.h>

class Battery {
	enum BatteryState {
		NOMINAL = 0x00U,
		WARNING,
		CRITICAL,
		UNKNOWN = 0xFFU
	};

public:
	Battery(const uint8_t gpio);
	~Battery() = default;

	BatteryState GetState(void);
	uint16_t GetVoltage(void);

	void Initialize(void);
	void Update(const uint32_t currentTime);

	bool BuildFrameVoltage(Frame&response);
	bool BuildFrameState(Frame&response);

private:
	uint16_t mVoltage;
	BatteryState mState;
	BatteryState mOldState;
	Adc mAdc;
};
