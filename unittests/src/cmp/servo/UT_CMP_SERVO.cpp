#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/cmp/Servo.h"

using ::testing::StrictMock;
using ::testing::_;

using namespace Component;

TEST( ComponentServo, Initialize_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U );

	success = servo.Initialize();

	EXPECT_TRUE( success );
}

TEST( ComponentServo, Update_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U );
	EXPECT_CALL( pca9685, SetPwm( 0U, _ ) ).Times( 1U );

	success = servo.Initialize();
	servo.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST( ComponentServo, SetAngle_Ok )
{
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U );
	servo.Initialize();

	EXPECT_TRUE( servo.SetAngle( 10U ) );
	EXPECT_TRUE( servo.IsMoving() );
	EXPECT_EQ( servo.GetAngle(), 10U );
}

TEST( ComponentServo, SetAngle_Reverse_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;


	Servo servo( pca9685, tick, 0U, 90U, 0, 0U, 180U, true );
	servo.Initialize();

	success = servo.Initialize();

	EXPECT_TRUE( servo.SetAngle( 70U ) );
	EXPECT_TRUE( servo.IsMoving() );
	EXPECT_EQ( servo.GetAngle(), 110U );
	EXPECT_TRUE( success );
}

TEST( ComponentServo, SetAngle_AboveMax_Ko )
{
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U, 90U, 0, 0U, 180U );
	servo.Initialize();

	EXPECT_FALSE( servo.SetAngle( 190U ) );
	EXPECT_EQ( servo.GetAngle(), 90U );
	EXPECT_FALSE( servo.IsMoving() );
}

TEST( ComponentServo, SetAngle_BelowMin_Ko )
{
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U, 90U, 0, 50U, 180U );
	servo.Initialize();

	EXPECT_FALSE( servo.SetAngle( 40U ) );
	EXPECT_EQ( servo.GetAngle(), 90U );
	EXPECT_FALSE( servo.IsMoving() );
}

TEST( ComponentServo, SetAngle_AboveMax_WithOffset_Ko )
{
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U, 90U, -10, 0U, 180U );
	servo.Initialize();

	EXPECT_FALSE( servo.SetAngle( 180U ) );
	EXPECT_EQ( servo.GetAngle(), 90U );
	EXPECT_FALSE( servo.IsMoving() );
}

TEST( ComponentServo, SetAngle_BelowMin_WithOffset_Ko )
{
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U, 90U, 10, 50U, 180U );
	servo.Initialize();

	EXPECT_FALSE( servo.SetAngle( 50U ) );
	EXPECT_EQ( servo.GetAngle(), 90U );
	EXPECT_FALSE( servo.IsMoving() );
}

TEST( ComponentServo, SetMin_Ok )
{
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U );
	servo.Initialize();
	EXPECT_TRUE( servo.SetMin( 50U ) );
	EXPECT_EQ( servo.GetMin(), 50U );
}

TEST( ComponentServo, SetMax_Ok )
{
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	Servo servo( pca9685, tick, 0U );
	servo.Initialize();

	EXPECT_TRUE( servo.SetMax( 90U ) );
	EXPECT_EQ( servo.GetMax(), 90U );
}
