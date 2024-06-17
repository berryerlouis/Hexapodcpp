#include <avr/io.h>
#include "Gpio.h"

namespace Driver {
Gpio::GpioRegister Gpio::sGpioRegisters[] = {
	{ &PINA, &PORTA, &DDRA, &PCICR, &PCMSK0, PCIE0 },
	{ &PINB, &PORTB, &DDRB, &PCICR, &PCMSK1, PCIE1 },
	{ &PINC, &PORTC, &DDRC, &PCICR, &PCMSK2, PCIE2 },
	{ &PIND, &PORTD, &DDRD, &PCICR, &PCMSK3, PCIE3 },
};


Gpio::Gpio( const SGpio &gpio, const EPortDirection &portDirection ) :
	mGpio( gpio ),
	mPortDirection( portDirection ),
	mGpioRegister( Gpio::sGpioRegisters[gpio.port] )
{
	if ( this->mPortDirection == EPortDirection::OUT )
	{
		*( this->mGpioRegister.portDirPtr ) |= ( 1U << this->mGpio.pin );
	}
	else
	{
		*( this->mGpioRegister.portDirPtr ) &= ~( 1U << this->mGpio.pin );
	}
}

EPin &Gpio::GetPin ( void )
{
	return ( this->mGpio.pin );
}

Core::CoreStatus Gpio::Set ( void )
{
	*( this->mGpioRegister.portPtr ) |= ( 1U << this->mGpio.pin );
	return ( Core::CoreStatus::CORE_OK );
}

Core::CoreStatus Gpio::Reset ( void )
{
	*( this->mGpioRegister.portPtr ) &= ~( 1U << this->mGpio.pin );
	return ( Core::CoreStatus::CORE_OK );
}

bool Gpio::Get ( void )
{
	uint8_t pin_value = 0U;

	pin_value  = *( this->mGpioRegister.pinPtr );
	pin_value  = pin_value >> this->mGpio.pin;
	pin_value &= 0x01U;

	return ( pin_value );
}

void Gpio::SetInterruptPin ( void )
{
	*( this->mGpioRegister.pcicr ) |= ( 1U << this->mGpioRegister.pcie );
	*( this->mGpioRegister.pcmsk ) |= ( 1U << this->mGpio.pin );
}

void Gpio::ResetInterruptPin ( void )
{
	*( this->mGpioRegister.pcicr ) &= ~( 1U << this->mGpioRegister.pcie );
	*( this->mGpioRegister.pcmsk ) &= ~( 1U << this->mGpio.pin );
}
}
