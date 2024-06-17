#include "Adc.h"
#include "Isr.h"

namespace Driver {
volatile uint16_t Adc::sAdcValue = 0U;

Adc::Adc( GpioInterface & gpio )
	: mGpio( gpio )
{
}

Core::CoreStatus Adc::Initialize ( void )
{
	ADMUX  = ( 1 << REFS0 ) | ( 1 << REFS1 );
	ADCSRA = ( 1 << ADEN ) | ( 1 << ADPS0 ) | ( 1 << ADPS1 ) | ( 1 << ADIE );
	ADMUX  = ( ( ADMUX & 0xE0U ) | (uint8_t) this->mGpio.GetPin() );

	this->StartConversion();
	return ( Core::CoreStatus::CORE_OK );
}

void Adc::Update ( const uint32_t currentTime )
{
	(void) currentTime;
}

void Adc::StartConversion ( void )
{
	ADCSRA |= ( 1U << ADSC );
}

uint16_t Adc::Read ()
{
	this->StartConversion();
	return ( (uint16_t) ( 100.0 * ADC_VOLT( Adc::sAdcValue / BRIDGE_DIVIDER ) ) );
}

ISR( ADC_vect )
{
	ISR_EMBEDDED_CODE( Adc::sAdcValue = ADC; );
}
}
