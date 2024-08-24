#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Display/Ssd1306Interface.h"
#include "../../../src/Misc/Bitmap/Bitmaps.h"

namespace Component {
namespace Display {
using namespace Misc;
class MockSsd1306 : public Ssd1306Interface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD3( DrawPixel, void ( uint16_t x, uint16_t y, uint16_t color ) );
	MOCK_METHOD5( DrawLine, void ( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) );
	MOCK_METHOD5( DrawRectangle, void ( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color ) );
	MOCK_METHOD4( DrawCircle, void ( uint16_t xc, uint16_t yc, uint16_t r, uint16_t color ) );
	MOCK_METHOD4( DrawChar, void ( char c, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color ) );
	MOCK_METHOD4( DrawString, void ( const char *str, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color ) );
	MOCK_METHOD4( DrawBitmap, void ( const Bitmap::Bitmaps::SBitmap *bmp, uint16_t x, uint16_t y, Bitmap::Bitmaps::Color color ) );
	MOCK_METHOD0( ClearBuffer, void ( void ) );
	MOCK_METHOD4( EraseArea, void ( uint16_t x, uint16_t y, uint16_t width, uint16_t height ) );
};
}
}
