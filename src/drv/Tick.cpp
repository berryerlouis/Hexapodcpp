#include "Tick.h"
#include "Isr.h"

namespace Driver {
volatile uint64_t Tick::tick = 0U;

#define TICK_TIMER    ( 64.0F / ( F_CPU / 1000000.0F ) )


Tick::Tick( void )
{
	Tick::tick = 0U;
	TCCR0A     = 0U;
	TCNT0      = 130U;
	TCCR0B     = _BV( CS01 ) | _BV( CS00 );
	TIMSK0     = _BV( TOIE0 );
	
}

uint64_t Tick::GetUs ( void )
{
	return ( static_cast <uint64_t>( ( ( Tick::tick * 255U ) + TCNT0 ) * TICK_TIMER ) );
}

uint64_t Tick::GetMs ( void )
{
	double currentTime = GetUs();

	return ( static_cast <uint64_t>( currentTime / 1000U ) );
}

void Tick::DelayMs ( uint64_t delayMs )
{
	DelayUs( delayMs * 1000UL );
}

void Tick::DelayUs ( uint64_t delayUs )
{
	uint64_t timeToWait = GetUs() + delayUs;

	while ( GetUs() < timeToWait )
	{
	}
}

ISR( TIMER0_OVF_vect )
{
	ISR_EMBEDDED_CODE( Tick::tick += 1UL; );
}
}
