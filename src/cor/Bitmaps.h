#pragma once

#ifndef GTEST
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include <stdint.h>
#include "Pixel.h"

class Bitmaps
{
public:
	static int myArray[];
	static const uint8_t PROGMEM Battery100[];
	static const uint8_t PROGMEM Battery75[];
	static const uint8_t PROGMEM Battery50[];
	static const uint8_t PROGMEM Battery25[];
	static const uint8_t PROGMEM Battery0[];
	static const uint8_t PROGMEM ArrowRight[];
	static const uint8_t PROGMEM ArrowLeft[];
	static const uint8_t PROGMEM ArrowUp[];
	static const uint8_t PROGMEM ArrowDown[];
	static const uint8_t PROGMEM ArrowCenter[];
	static const uint8_t PROGMEM Communication[];
};
