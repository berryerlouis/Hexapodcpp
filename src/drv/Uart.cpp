#include "Uart.h"
#include "Isr.h"

namespace Driver {
Buffer Uart::bufferRx;
Buffer Uart::bufferTx;

Uart::Uart( const EBaudRate & baud ) : mBaudRate( baud )
{
}

Core::CoreStatus Uart::Initialize ( void )
{
	const uint16_t ubrr = ( F_CPU / ( this->mBaudRate * 8.0 ) ) - 1;

	UBRR0  = ubrr;
	UCSR0A = _BV( U2X0 );
	UCSR0B = _BV( TXEN0 ) | _BV( RXEN0 ) | _BV( RXCIE0 );
	UCSR0C = _BV( UCSZ01 ) | _BV( UCSZ00 );

	return ( Core::CoreStatus::CORE_OK );
}

void Uart::Update ( const uint64_t currentTime )
{
	(void) currentTime;
}

void Uart::Send ( const char *data, const size_t len )
{
	Send( (uint8_t *) data, len );
}

void Uart::Send ( const uint8_t *data, const size_t len )
{
	for ( size_t i = 0U; i < len; ++i )
	{
		Send( data[i] );
	}
}

void Uart::Send ( const uint8_t data )
{
	bufferTx.Push( data );
	UCSR0B |= _BV( UDRIE0 );
}

uint8_t Uart::Read ( void )
{
	return ( Uart::bufferRx.Pop() );
}

uint8_t Uart::DataAvailable ( void ) const
{
	return ( Uart::bufferRx.GetLength() );
}

ISR( USART0_UDRE_vect )
{
	ISR_EMBEDDED_CODE(
		if ( Uart::bufferTx.GetLength() > 0U )
		{
			UDR0 = Uart::bufferTx.Pop();
		}
		else
		{
			UCSR0B &= ~_BV( UDRIE0 );
		}
		);
}

ISR( USART0_RX_vect )
{
	ISR_EMBEDDED_CODE(
		volatile uint8_t receivedData = UDR0;

		if ( ( UCSR0A & ( _BV( FE0 ) | _BV( DOR0 ) | _BV( UPE0 ) ) ) == 0 )
		{
			if ( ( receivedData == 60 || receivedData == 62 ) ||
				  ( receivedData >= 48 && receivedData <= 57 ) ||
				  ( receivedData >= 65 && receivedData <= 70 ) ||
				  ( receivedData >= 97 && receivedData <= 102 ) )
			{
				Uart::bufferRx.Push( receivedData );
			}
		}
		);
}
}
