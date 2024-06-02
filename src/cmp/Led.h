#pragma once

#include <Arduino.h>

class Led {
public:
	enum LedState {
		ON = 0,
		OFF
	};

	Led(const uint8_t gpio);
	~Led() = default;


	bool On();
	bool Off();
	bool Toggle();
	LedState Get() const;

private:
	const uint8_t mIo;
	LedState mState;
};
