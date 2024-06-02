#pragma once

#include "DriverInterface.h"
#include <Arduino.h>

class Adc : public DriverInterface {
public:
#define BRIDGE_DIVIDER    (float)(0.2F)
#define ADC_STEP          (float)(5.0 / 1024)
#define ADC_VOLT(ADC_VALUE)    (float)(ADC_STEP * ADC_VALUE)

	Adc(const uint8_t gpio);
	~Adc() = default;

	virtual void Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	uint16_t Read(void);

private:
	const uint8_t mIo;
};
