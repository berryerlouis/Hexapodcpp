#pragma once

#ifndef GTEST
#include <avr/pgmspace.h>
#else
#define PROGMEM
#endif

#include <stdint.h>
#include "Pixel.h"

struct Bitmaps
{
	inline static const uint8_t PROGMEM bitmapBattery100[14U] =
	{
		XXXXXXXX, XXXXXXX_,
		X_______, ______X_,
		X_XXX_XX, X_XXX_XX,
		X_XX_XXX, _XXX__XX,
		X_X_XXX_, XXX_X_XX,
		X_______, ______X_,
		XXXXXXXX, XXXXXXX_
	};

	inline static const uint8_t PROGMEM bitmapBattery75[14U] =
	{
		XXXXXXXX, XXXXXXX_,
		X_______, ______X_,
		X_XXX_XX, X_XXX_XX,
		X_XX_XXX, _XXX__XX,
		X_X_XXX_, XXX___XX,
		X_______, ______X_,
		XXXXXXXX, XXXXXXX_
	};
	inline static const uint8_t PROGMEM bitmapBattery50[14U] =
	{
		XXXXXXXX, XXXXXXX_,
		X_______, ______X_,
		X_XXX_XX, X_____XX,
		X_XX_XXX, ______XX,
		X_X_XXX_, ______XX,
		X_______, ______X_,
		XXXXXXXX, XXXXXXX_
	};
	inline static const uint8_t PROGMEM bitmapBattery25[14U] =
	{
		XXXXXXXX, XXXXXXX_,
		X_______, ______X_,
		X_XXX___, ______XX,
		X_XX____, ______XX,
		X_X_____, ______XX,
		X_______, ______X_,
		XXXXXXXX, XXXXXXX_
	};
	inline static const uint8_t PROGMEM bitmapBattery0[14U] =
	{
		XXXXXXXX, XXXXXXX_,
		X_______, ______X_,
		X_______, ______XX,
		X_______, ______XX,
		X_______, ______XX,
		X_______, ______X_,
		XXXXXXXX, XXXXXXX_
	};
	inline static const uint8_t PROGMEM bitmapArrowRight[12U] =
	{
		________, _XXX____,
		________, __XXX___,
		__XXXXXX, XXXXXX__,
		__XXXXXX, XXXXXX__,
		________, __XXX___,
		________, _XXX____
	};
	inline static const uint8_t PROGMEM bitmapArrowLeft[12U] =
	{
		____XXX_, ________,
		___XXX__, ________,
		__XXXXXX, XXXXXX__,
		__XXXXXX, XXXXXX__,
		___XXX__, ________,
		____XXX_, ________
	};
	inline static const uint8_t PROGMEM bitmapArrowUp[12U] =
	{
		_______X, X_______,
		______XX, XX______,
		_____XXX, XXX_____,
		_______X, X_______,
		_______X, X_______,
		_______X, X_______
	};
	inline static const uint8_t PROGMEM bitmapArrowDown[12U] =
	{
		_______X, X_______,
		_______X, X_______,
		_______X, X_______,
		_____XXX, XXX_____,
		______XX, XX______,
		_______X, X_______
	};
	inline static const uint8_t PROGMEM bitmapArrowCenter[12U] =
	{
		_______X, X_______,
		_____XXX, XXX_____,
		____XXXX, XXXX____,
		____XXXX, XXXX____,
		_____XXX, XXX_____,
		_______X, X_______
	};
	inline static const uint8_t PROGMEM bitmapCommunication[16U] =
	{
		________, ________,
		____XXX_, ____XX__,
		___XX___, ___XXXX_,
		__XX____, __XXXXXX,
		XXXXXX__, ____XX__,
		_XXXX___, ___XX___,
		__XX____, _XXX____,
		________, ________
	};
};
