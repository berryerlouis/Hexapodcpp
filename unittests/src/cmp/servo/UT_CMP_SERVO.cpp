#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/cmp/Servo.h"

using ::testing::StrictMock;
using ::testing::_;

using namespace Component;

TEST(ComponentServo, Initialize_Ok)
{
	bool success = false;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo(pca9685, tick, 0U);
	EXPECT_CALL(tick, GetMs() ).Times(1U);

	success = servo.Initialize();

	EXPECT_TRUE(success);
}

TEST(ComponentServo, Update_Ok)
{
	bool success = false;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo(pca9685, tick, 0U);
	EXPECT_CALL(tick, GetMs() ).Times(1U);
	EXPECT_CALL(pca9685, SetPwm(0U, _) ).Times(1U);

	success = servo.Initialize();
	servo.Update(0UL);

	EXPECT_TRUE(success);
}


TEST(ComponentServo, SetAngle_Ok)
{
	bool success = false;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo(pca9685, tick, 0U);
	EXPECT_CALL(tick, GetMs() ).Times(2U);

	success = servo.Initialize();

	EXPECT_TRUE(servo.SetAngle(0UL) );
	EXPECT_TRUE(success);
}

TEST(ComponentServo, SetMin_Ok)
{
	bool success = false;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo(pca9685, tick, 0U);
	EXPECT_CALL(tick, GetMs() ).Times(1U);

	success = servo.Initialize();

	EXPECT_TRUE(servo.SetMin(0UL) );
	EXPECT_TRUE(success);
}

TEST(ComponentServo, SetMax_Ok)
{
	bool success = false;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo(pca9685, tick, 0U);
	EXPECT_CALL(tick, GetMs() ).Times(1U);

	success = servo.Initialize();

	EXPECT_TRUE(servo.SetMax(90UL) );
	EXPECT_TRUE(success);
}
