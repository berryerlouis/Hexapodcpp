#include "Tick.h"
#include "Isr.h"

namespace Driver {
volatile uint64_t Tick::tick = 0U;

#define TICK_TIMER    ( 64.0 / ( F_CPU / 1000000.0 ) )


Tick::Tick( void )
{
	Tick::tick = 0U;
	TCCR0A     = 0U;
	TCNT0      = 130U;
	TCCR0B     = ( 1 << CS01 ) | ( 1 << CS00 );
	TIMSK0     = ( 1 << TOIE0 );
}

uint64_t Tick::GetUs ( void )
{
	return ( (uint64_t) ( ( Tick::tick * 255U ) + TCNT0 ) * TICK_TIMER );
}

uint32_t Tick::GetMs ( void )
{
	double currentTime = GetUs();

	return ( (uint32_t) ( (uint64_t) ( currentTime / 1000U ) ) );
}

void Tick::DelayMs ( uint16_t delayMs )
{
	DelayUs( delayMs * 1000UL );
}

void Tick::DelayUs ( uint32_t delayUs )
{
	uint32_t timeToWait = GetUs() + delayUs;

	while ( GetUs() < timeToWait )
	{
	}
}

ISR( TIMER0_OVF_vect )
{
	ISR_EMBEDDED_CODE( Tick::tick += 1UL; );
}
}
