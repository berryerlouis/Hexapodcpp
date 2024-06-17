#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockAdc.h"
#include "../../../../src/cmp/Battery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;


TEST( ComponentBattery, Initialize_Ok )
{
	Core::CoreStatus     success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockAdc> adc;
	Battery battery( adc );

	EXPECT_CALL( adc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = battery.Initialize();

	EXPECT_TRUE( success );
}

TEST( ComponentBattery, GetStateAfterInit )
{
	Core::CoreStatus     success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockAdc> adc;
	Battery battery( adc );

	EXPECT_CALL( adc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = battery.Initialize();
	Battery::BatteryState state = battery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, Battery::BatteryState::UNKNOWN );
}


TEST( ComponentBattery, GetStateAfterUpdateCritical )
{
	Core::CoreStatus     success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockAdc> adc;
	Battery battery( adc );

	EXPECT_CALL( adc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( adc, Read() ).WillOnce( Return( 74U ) );

	success = battery.Initialize();
	battery.Update( 0UL );
	Battery::BatteryState state = battery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, Battery::BatteryState::CRITICAL );
}

TEST( ComponentBattery, GetStateAfterUpdateWarning )
{
	Core::CoreStatus     success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockAdc> adc;
	Battery battery( adc );

	EXPECT_CALL( adc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( adc, Read() ).WillOnce( Return( 79U ) );

	success = battery.Initialize();
	battery.Update( 0UL );
	Battery::BatteryState state = battery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, Battery::BatteryState::WARNING );
}

TEST( ComponentBattery, GetStateAfterUpdateNominal )
{
	Core::CoreStatus     success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockAdc> adc;
	Battery battery( adc );

	EXPECT_CALL( adc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( adc, Read() ).WillOnce( Return( 90U ) );

	success = battery.Initialize();
	battery.Update( 0UL );
	Battery::BatteryState state = battery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, Battery::BatteryState::NOMINAL );
}
