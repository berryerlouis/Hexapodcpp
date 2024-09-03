#pragma once

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define ISR_EMBEDDED_CODE( code ) \
          (                                 \
    {                                         \
        uint8_t oldSREG = SREG;         \
        cli();                             \
        code                                 \
        SREG = oldSREG;                 \
        sei();                             \
    }                                         \
          )
