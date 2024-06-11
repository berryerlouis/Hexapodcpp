#include "Uart.h"
#include "Isr.h"

Buffer Uart::bufferRx;

Uart::Uart(const EBaudRate & baud) : mBaudRate(baud)
{
}

bool Uart::Initialize (void)
{
	const uint16_t ubrr = (F_CPU / (this->mBaudRate * 8.0) ) - 1;

	UBRR0  = ubrr;
	UCSR0A = (1 << U2X0);
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

	return (true);
}

void Uart::Update (const uint32_t currentTime)
{
	(void) currentTime;
}

void Uart::Send (const char *data, const size_t len)
{
	Send( (uint8_t *) data, len);
}

void Uart::Send (const uint8_t *data, const size_t len)
{
	for (size_t i = 0; i < len; ++i)
	{
		Send(data[i]);
	}
}

void Uart::Send (const uint8_t data)
{
	while (!(UCSR0A & (1 << UDRE0) ) )
	{
		;
	}
	UDR0 = data;
}

uint8_t Uart::Read (void)
{
	return (Uart::bufferRx.Pop() );
}

uint8_t Uart::DataAvailable (void) const
{
	return (Uart::bufferRx.GetLength() );
}

ISR(USART0_RX_vect)
{
	ISR_EMBEDDED_CODE(
		volatile uint8_t receivedData = UDR0;

		if ( (UCSR0A & ( (1 << FE0) | (1 << DOR0) | (1 << UPE0) ) ) == 0)
	{
		if ( (receivedData == 60 || receivedData == 62) ||
			  (receivedData >= 48 && receivedData <= 57) ||
			  (receivedData >= 65 && receivedData <= 70) ||
			  (receivedData >= 97 && receivedData <= 102) )
		{
			Uart::bufferRx.Push(receivedData);
		}
	}
		);
}


Uart MySerial;
