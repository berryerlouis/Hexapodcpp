#pragma once

#include "../ComponentInterface.h"
#include "../../Misc/Bitmap/Bitmaps.h"

namespace Component {
namespace Display {
using namespace Misc;

#define SCREEN_WIDTH     128U                                // OLED display width, in pixels
#define SCREEN_HEIGHT    32U                                 // OLED display height, in pixels

class Ssd1306Interface : public ComponentInterface {
public:

	Ssd1306Interface()  = default;
	~Ssd1306Interface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual void DrawPixel( uint16_t x, uint16_t y, uint16_t color ) = 0;
	virtual void DrawLine( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color )      = 0;
	virtual void DrawRectangle( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) = 0;
	virtual void DrawCircle( uint16_t xc, uint16_t yc, uint16_t r, uint16_t color )                  = 0;
	virtual void DrawChar( char c, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color )            = 0;
	virtual void DrawString( const char *str, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color ) = 0;
	virtual void DrawBitmap( const Bitmap::Bitmaps::SBitmap *bmp, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color ) = 0;
	virtual void ClearBuffer( void ) = 0;
	virtual void EraseArea( uint16_t x, uint16_t y, uint16_t width, uint16_t height ) = 0;
};
}
}
