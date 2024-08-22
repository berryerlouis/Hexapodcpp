#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/Ssd1306Interface.h"

namespace Component {
class MockSsd1306 : public Ssd1306Interface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );


	MOCK_METHOD3( DrawPixel, void ( uint16_t x, uint16_t y, uint16_t color ) );
	MOCK_METHOD5( DrawLine, void ( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) );
	MOCK_METHOD5( DrawRectangle, void ( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) );
	MOCK_METHOD4( DrawCircle, void ( uint16_t xc, uint16_t yc, uint16_t r, uint16_t color ) );
	MOCK_METHOD4( DrawChar, void ( char c, uint16_t x, uint16_t y, Ssd1306Interface::Color color ) );
	MOCK_METHOD4( DrawString, void ( const char *str, uint16_t x, uint16_t y, Ssd1306Interface::Color color ) );
	MOCK_METHOD4( DrawBitmap, void ( const Ssd1306Interface::SBitmap * bmp, uint16_t x, uint16_t y, Ssd1306Interface::Color color ) );
	MOCK_METHOD0( ClearBuffer, void ( void ) );
	MOCK_METHOD4( EraseArea, void ( uint16_t x, uint16_t y, uint16_t width, uint16_t height ) );
};
}
