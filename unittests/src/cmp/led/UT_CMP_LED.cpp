#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockGpio.h"
#include "../../../../src/cmp/Led.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;


TEST( ComponentLed, Initialize_Ok )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockGpio> gpio;
	Led led( gpio );

	EXPECT_CALL( gpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = led.Initialize();

	EXPECT_TRUE( success );
}

TEST( ComponentLed, On )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockGpio> gpio;
	Led led( gpio );

	EXPECT_CALL( gpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( gpio, Reset() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = led.Initialize();
	led.On();

	Led::LedState status = led.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::ON );
}

TEST( ComponentLed, Off )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockGpio> gpio;
	Led led( gpio );

	EXPECT_CALL( gpio, Set() ).Times( 2U ).WillRepeatedly( Return( Core::CoreStatus::CORE_OK ) );

	success = led.Initialize();
	led.Off();

	Led::LedState status = led.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::OFF );
}

TEST( ComponentLed, Toggle )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockGpio> gpio;
	Led led( gpio );

	EXPECT_CALL( gpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( gpio, Reset() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = led.Initialize();
	led.Toggle();

	Led::LedState status = led.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::ON );
}

TEST( ComponentLed, Get )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockGpio> gpio;
	Led led( gpio );

	EXPECT_CALL( gpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = led.Initialize();
	Led::LedState status = led.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::OFF );
}
