#pragma once

#include "GpioInterface.h"
#include "AdcInterface.h"

namespace Driver {
class Adc : public AdcInterface {
public:
#define BRIDGE_DIVIDER    (float) ( 0.2F )
#define ADC_STEP          (float) ( 5.0 / 1024 )
#define ADC_VOLT( ADC_VALUE )    (float) ( ADC_STEP * ADC_VALUE )

	Adc( GpioInterface &gpio );
	~Adc() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint32_t currentTime ) final override;

	virtual void StartConversion( void ) final override;
	virtual uint16_t Read( void ) final override;

	static volatile uint16_t sAdcValue;
private:
	GpioInterface &mGpio;
};
}
