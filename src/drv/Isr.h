#pragma once

#include <stdint.h>
#include <avr/interrupt.h>
namespace Driver {
#define ISR_EMBEDDED_CODE(code) \
	(									  \
	{									  \
		uint8_t oldSREG = SREG;	  \
		cli();						  \
		code							  \
		SREG = oldSREG;			  \
	}									  \
	)
}
