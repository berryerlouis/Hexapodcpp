#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockGpio.h"
#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/drv/MockInputCapture.h"
#include "../../../mock/drv/MockTick.h"
#include "../../../../src/cmp/Srf05.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST( ComponentSrf05, Initialize_Ok )
{
	Core::CoreStatus              success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;
	StrictMock <MockLed>          led;

	Srf05 srf05( EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, led, tick );

	EXPECT_CALL( gpioInputCaptureEcho, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( led, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = srf05.Initialize();

	EXPECT_EQ( srf05.GetThreshold(), 30U );
	EXPECT_TRUE( success );
}

TEST( ComponentSrf05, Update_Ok )
{
	Core::CoreStatus              success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;
	StrictMock <MockLed>          led;

	Srf05 srf05( EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, led, tick );

	EXPECT_CALL( gpioInputCaptureEcho, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( gpioTrigger, Set() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( tick, DelayUs( _ ) ).Times( 1U );
	EXPECT_CALL( gpioTrigger, Reset() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( led, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = srf05.Initialize();
	srf05.Update( 0U );

	EXPECT_TRUE( success );
}

TEST( ComponentSrf05, IsDetecting_SetThreshold )
{
	Core::CoreStatus              success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;
	StrictMock <MockLed>          led;

	Srf05 srf05( EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, led, tick );

	EXPECT_CALL( gpioInputCaptureEcho, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( led, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = srf05.Initialize();

	EXPECT_TRUE( srf05.SetThreshold( 35U ) );
	EXPECT_EQ( 35U, srf05.GetThreshold() );
	EXPECT_TRUE( success );
}

TEST( ComponentSrf05, IsDetecting_UnderThreshold )
{
	Core::CoreStatus              success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;
	StrictMock <MockLed>          led;

	Srf05 srf05( EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, led, tick );

	EXPECT_CALL( gpioInputCaptureEcho, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( gpioInputCaptureEcho, GetInputCaptureTime() ).Times( 1U ).WillOnce( Return( 580U ) );
	EXPECT_CALL( led, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( led, On ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = srf05.Initialize();

	EXPECT_TRUE( srf05.IsDetecting() );
	EXPECT_TRUE( success );
}

TEST( ComponentSrf05, IsDetecting_AboveThreshold )
{
	Core::CoreStatus              success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;

	StrictMock <MockLed> led;

	Srf05 srf05( EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, led, tick );

	EXPECT_CALL( gpioInputCaptureEcho, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( gpioInputCaptureEcho, GetInputCaptureTime() ).Times( 1U ).WillOnce( Return( 58U * 31U ) );
	EXPECT_CALL( led, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( led, Off() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = srf05.Initialize();

	EXPECT_FALSE( srf05.IsDetecting() );
	EXPECT_TRUE( success );
}
