#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSsd1306.h"
#include "../../../mock/cmp/MockBattery.h"

#include "../../../../src/Service/Display/ServiceDisplay.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace Display {
class UT_SRV_DISPLAY : public ::testing::Test {
protected:
	UT_SRV_DISPLAY() :
		mServiceDisplay( mMockSsd1306, mMockBattery )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_DISPLAY() = default;

	/* Mocks */
	StrictMock <Component::Display::MockSsd1306> mMockSsd1306;
	StrictMock <Component::Battery::MockBattery> mMockBattery;

	/* Test class */
	ServiceDisplay mServiceDisplay;
};

TEST_F( UT_SRV_DISPLAY, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSsd1306, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockBattery, Attach( _ ) ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockSsd1306, DrawLine( 0, 10U, SCREEN_WIDTH, 10U, Bitmap::Bitmaps::Color::COLOR_WHITE ) ).Times( 1U );
	success = mServiceDisplay.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_DISPLAY, Update_Ok )
{
	bool toggle = false;

	for ( uint64_t i = 500U; i < 10U * 500U; i += 500U )
	{
		if ( toggle == true )
		{
			toggle = false;
			EXPECT_CALL( mMockSsd1306, DrawBitmap( _, _, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE ) ).Times( 1U );
		}
		else
		{
			toggle = true;
			EXPECT_CALL( mMockSsd1306, EraseArea( _, 0U, SCREEN_WIDTH, _ ) ).Times( 1U );
		}
		EXPECT_CALL( mMockSsd1306, Update( i ) ).Times( 1U );
		mServiceDisplay.Update( i );
	}
}
}
}
