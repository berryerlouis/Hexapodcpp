#include "Uart.h"
#include "../Isr.h"

namespace Driver
{
    namespace Uart
    {
	    Misc::Buffer::Buffer mBufferRx;
	    Misc::Buffer::Buffer mBufferTx;
		
        Uart::Uart(const EBaudRate &baud) 
		: mBaudRate(baud) {
        }

        Core::CoreStatus Uart::Initialize(void) {
            const uint16_t ubrr = (F_CPU / (this->mBaudRate * 8.0)) - 1;

            UBRR0 = ubrr;
            UCSR0A = _BV(U2X0);
            UCSR0B = _BV(TXEN0) | _BV(RXEN0) | _BV(RXCIE0);
            UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);

            return (Core::CoreStatus::CORE_OK);
        }

        void Uart::Update(const uint64_t currentTime) {
            (void) currentTime;
        }

        void Uart::Send(const char *data, const size_t len) {
            for (size_t i = 0U; i < len; ++i) {
                mBufferTx.Push(static_cast<const uint8_t &>(data[i]));
            }
            UCSR0B |= _BV(UDRIE0);
        }

        void Uart::Send(const uint8_t data) {
            mBufferTx.Push(data);
            UCSR0B |= _BV(UDRIE0);
        }

        uint8_t Uart::Read(void) {
            return mBufferRx.Pop();
        }

        uint8_t Uart::DataAvailable(void) const {
            return (mBufferRx.GetLength());
        }

        ISR(USART0_UDRE_vect) {
			UDR0 = mBufferTx.Pop();
            UCSR0A |= _BV(TXC0);
            if (mBufferTx.GetLength() == 0U) {
                UCSR0B &= ~_BV(UDRIE0);
            }
        }

        ISR(USART0_RX_vect) {
            volatile uint8_t receivedData = UDR0;
            if ((UCSR0A & (_BV(FE0) | _BV(DOR0) | _BV(UPE0))) == 0U) {
                mBufferRx.Push(receivedData);
            }
        }
    }
}
