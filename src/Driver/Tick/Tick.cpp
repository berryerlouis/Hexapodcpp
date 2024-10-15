#include "Tick.h"
#include "../Isr.h"
#include <avr/cpufunc.h>

namespace Driver
{
    namespace Tick
    {
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64U * 256U))
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000.0)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000U) >> 3U)
#define FRACT_MAX (1000U >> 3U)

        volatile uint64_t timer0_overflow_count = 0UL;
        volatile uint64_t timer0_millis = 0UL;
        static uint8_t timer0_fract = 0U;

        Tick::Tick(void) {
            TCCR0A = 0U;
            TCNT0 = 0U;
            TCCR0B = _BV(CS01) | _BV(CS00);
            TIMSK0 = _BV(TOIE0);
        }

        uint64_t Tick::GetUs(void) {
            volatile uint32_t ovf0;
            volatile uint8_t tcnt;

            ISR_EMBEDDED_CODE(
                ovf0 = timer0_overflow_count;
                tcnt = TCNT0;
                if ( ( TIFR0 & _BV( TOV0 ) ) && ( tcnt < 0xFFU ) )
                {
                ovf0++;
                }
            );
            return (((ovf0 << 8U) + tcnt) * (64U / clockCyclesPerMicrosecond()));;
        }

        uint64_t Tick::GetMs(void) {
            uint64_t currentTime = 0UL;
            ISR_EMBEDDED_CODE(currentTime = timer0_millis;);
            return (currentTime);
        }

        void Tick::DelayMs(const uint64_t delayMs) {
            uint64_t tempDelay = delayMs;
            uint32_t start = this->GetUs();
            while (tempDelay > 0) {
                while (tempDelay > 0 && (this->GetUs() - start) >= 1000) {
                    tempDelay--;
                    start += 1000;
                }
            }
        }

        void Tick::DelayUs(const uint64_t delayUs) {
            uint64_t tempDelay = delayUs;
            __asm__ __volatile__ (
                "nop" "\n\t"
                "nop" "\n\t"
                "nop" "\n\t"
                "nop"); //just waiting 4 cycles
            if (tempDelay <= 1) return; //  = 3 cycles, (4 when true)
            tempDelay = (tempDelay << 2) + tempDelay; // x5 us, = 7 cycles
            tempDelay -= 7; // 2 cycles
            __asm__ __volatile__ (
                "1: sbiw %0,1" "\n\t" // 2 cycles
                "brne 1b" : "=w" (tempDelay) : "0" (tempDelay) // 2 cycles
            );
        }

        ISR(TIMER0_OVF_vect) {
            // copy these to local variables so they can be stored in registers
            // (volatile variables must be read from memory on every access)
            unsigned long m = timer0_millis;
            unsigned char f = timer0_fract;

            m += MILLIS_INC;
            f += FRACT_INC;
            if (f >= FRACT_MAX) {
                f -= FRACT_MAX;
                m += 1;
            }

            timer0_fract = f;
            timer0_millis = m;
            timer0_overflow_count++;
        }
    }
}
