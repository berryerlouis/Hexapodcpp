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
	volatile uint32_t ovf0;
	volatile uint8_t  tcnt0;

	ATOMIC_EMBEDDED_CODE(
		ovf0  = Tick::tick;
		tcnt0 = TCNT0;
		if ( ( TIFR0 & _BV( TOV0 ) ) && ( tcnt0 < 0xFFU ) )
		{
			ovf0++;
		}
		);
	return ( static_cast <uint64_t>( ( ovf0 << 8U ) + tcnt0 ) / TICK_TIMER );
}

uint64_t Tick::GetMs ( void )
{
	uint64_t currentTime = GetUs();

	return ( static_cast <uint64_t>( currentTime / 100.0F ) );
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
