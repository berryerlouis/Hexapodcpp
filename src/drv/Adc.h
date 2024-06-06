#pragma once

#include "DriverInterface.h"
#include "Gpio.h"

class Adc : public DriverInterface {
public:
#define BRIDGE_DIVIDER    (float) (0.2F)
#define ADC_STEP          (float) (5.0 / 1024)
#define ADC_VOLT(ADC_VALUE)    (float) (ADC_STEP * ADC_VALUE)

	Adc(const SGpio & gpio);
	~Adc() = default;

	virtual void Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	void StartConversion(void);
	uint16_t Read(void);

	static volatile uint16_t sAdcValue;
private:
	Gpio mGpio;
};
