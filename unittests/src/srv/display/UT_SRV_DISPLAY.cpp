#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockSsd1306.h"

#include "../../../../src/srv/ServiceDisplay.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST( ServiceDisplay, Initialize_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSsd1306> ssd1306;
	ServiceDisplay           serviceDisplay( ssd1306 );

	EXPECT_CALL( ssd1306, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( ssd1306, DrawLine( 0, 10U, SCREEN_WIDTH, 10U, Ssd1306Interface::Color::COLOR_WHITE ) ).Times( 1U );
	success = serviceDisplay.Initialize();

	EXPECT_TRUE( success );
}

TEST( ServiceDisplay, Update_Ok )
{
	StrictMock <MockSsd1306> ssd1306;
	ServiceDisplay           serviceDisplay( ssd1306 );
	bool toggle = false;

	for ( uint64_t i = 500U; i < 10U * 500U; i += 500U )
	{
		if ( toggle == true )
		{
			toggle = false;
			EXPECT_CALL( ssd1306, DrawBitmap( _, _, 0U, Ssd1306Interface::Color::COLOR_WHITE ) ).Times( 1U );
		}
		else
		{
			toggle = true;
			EXPECT_CALL( ssd1306, EraseArea( _, 0U, SCREEN_WIDTH, _ ) ).Times( 1U );
		}
		EXPECT_CALL( ssd1306, Update( i ) ).Times( 1U );
		serviceDisplay.Update( i );
	}
}
