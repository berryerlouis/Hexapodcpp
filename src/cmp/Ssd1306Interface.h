#pragma once

#include "ComponentInterface.h"

namespace Component {
class Ssd1306Interface : public ComponentInterface {
public:
	struct SBitmap
	{
		uint8_t *bmp;
		uint16_t width;
		uint16_t height;
	};

	enum Color
	{
		COLOR_BLACK,
		COLOR_WHITE,
	};

	Ssd1306Interface()  = default;
	~Ssd1306Interface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual void DrawPixel( uint16_t x, uint16_t y, uint16_t color ) = 0;
	virtual void DrawLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color )      = 0;
	virtual void DrawRectangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) = 0;
	virtual void DrawCircle( uint16_t xc, uint16_t yc, uint16_t r, uint16_t color )    = 0;
	virtual void DrawChar( char c, uint16_t x, uint16_t y, Color color )               = 0;
	virtual void DrawString( const char *str, uint16_t x, uint16_t y, Color color )    = 0;
	virtual void DrawBitmap( const SBitmap *bmp, uint16_t x, uint16_t y, Color color ) = 0;
	virtual void ClearBuffer( void ) = 0;
	virtual void EraseArea( uint16_t x, uint16_t y, uint16_t width, uint16_t height ) = 0;
};
}
