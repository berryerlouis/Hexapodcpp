#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockGpio.h"
#include "../../../mock/drv/MockInputCapture.h"
#include "../../../mock/drv/MockTick.h"
#include "../../../../src/cmp/Srf05.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST(ComponentSrf05, Initialize_Ok)
{
	bool success = false;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;

	Srf05 srf05(EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, tick);

	EXPECT_CALL(gpioInputCaptureEcho, Initialize() ).WillOnce(Return(true) );

	success = srf05.Initialize();

	EXPECT_EQ(srf05.GetThreshold(), 30U);
	EXPECT_TRUE(success);
}

TEST(ComponentSrf05, Update_Ok)
{
	bool success = false;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;

	Srf05 srf05(EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, tick);

	EXPECT_CALL(gpioInputCaptureEcho, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(gpioTrigger, Set() ).WillOnce(Return(true) );
	EXPECT_CALL(tick, DelayUs(_) ).Times(1U);
	EXPECT_CALL(gpioTrigger, Reset() ).WillOnce(Return(true) );

	success = srf05.Initialize();
	srf05.Update(0U);

	EXPECT_TRUE(success);
}

TEST(ComponentSrf05, IsDetecting_SetThreshold)
{
	bool success = false;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;

	Srf05 srf05(EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, tick);

	EXPECT_CALL(gpioInputCaptureEcho, Initialize() ).WillOnce(Return(true) );

	success = srf05.Initialize();

	EXPECT_TRUE(srf05.SetThreshold(35U) );
	EXPECT_EQ(35U, srf05.GetThreshold() );
	EXPECT_TRUE(success);
}



TEST(ComponentSrf05, IsDetecting_UnderThreshold)
{
	bool success = false;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;

	Srf05 srf05(EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, tick);

	EXPECT_CALL(gpioInputCaptureEcho, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(gpioInputCaptureEcho, GetInputCaptureTime() ).WillOnce(Return(580U) );

	success = srf05.Initialize();

	EXPECT_TRUE(srf05.IsDetecting() );
	EXPECT_TRUE(success);
}

TEST(ComponentSrf05, IsDetecting_AboveThreshold)
{
	bool success = false;
	StrictMock <MockTick>         tick;
	StrictMock <MockGpio>         gpioTrigger;
	StrictMock <MockInputCapture> gpioInputCaptureEcho;

	Srf05 srf05(EProximityCommands::US_LEFT, gpioTrigger, gpioInputCaptureEcho, tick);

	EXPECT_CALL(gpioInputCaptureEcho, Initialize() ).WillOnce(Return(true) );
	EXPECT_CALL(gpioInputCaptureEcho, GetInputCaptureTime() ).WillOnce(Return(58U * 31U) );

	success = srf05.Initialize();

	EXPECT_FALSE(srf05.IsDetecting() );
	EXPECT_TRUE(success);
}
