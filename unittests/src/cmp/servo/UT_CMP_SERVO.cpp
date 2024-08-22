#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/cmp/Servo.h"

using ::testing::StrictMock;
using ::testing::_;

using namespace Component;

class UT_CMP_SERVO : public ::testing::Test {
protected:
	UT_CMP_SERVO() :
		mMockTick(),
		mMockPca9685(),
		mServo( mMockPca9685, mMockTick, 0U, 90U, 0, 0U, 180U, false )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_SERVO() = default;

	/* Mocks */
	StrictMock <MockTick> mMockTick;
	StrictMock <MockPca9685> mMockPca9685;

	/* Test class */
	Servo mServo;
};

TEST_F( UT_CMP_SERVO, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	success = mServo.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_SERVO, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockPca9685, SetPwm( 0U, _ ) ).Times( 1U );

	success = mServo.Initialize();
	mServo.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_SERVO, SetAngle_Ok )
{
	mServo.Initialize();

	EXPECT_TRUE( mServo.SetAngle( 10U ) );
	EXPECT_TRUE( mServo.IsMoving() );
	EXPECT_EQ( mServo.GetAngle(), 10U );
}

TEST_F( UT_CMP_SERVO, SetAngle_Reverse_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	mServo.SetReverse( true );
	mServo.Initialize();

	success = mServo.Initialize();

	EXPECT_TRUE( mServo.SetAngle( 70U ) );
	EXPECT_TRUE( mServo.IsMoving() );
	EXPECT_EQ( mServo.GetAngle(), 110U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_SERVO, SetAngle_AboveMax_Ko )
{
	mServo.Initialize();

	EXPECT_FALSE( mServo.SetAngle( 190U ) );
	EXPECT_EQ( mServo.GetAngle(), 90U );
	EXPECT_FALSE( mServo.IsMoving() );
}

TEST_F( UT_CMP_SERVO, SetAngle_BelowMin_Ko )
{
	mServo.SetMin( 50U );
	mServo.Initialize();

	EXPECT_FALSE( mServo.SetAngle( 40U ) );
	EXPECT_EQ( mServo.GetAngle(), 90U );
	EXPECT_FALSE( mServo.IsMoving() );
}

TEST_F( UT_CMP_SERVO, SetAngle_AboveMax_WithOffset_Ko )
{
	mServo.SetOffset( -10 );
	mServo.Initialize();

	EXPECT_FALSE( mServo.SetAngle( 180U ) );
	EXPECT_EQ( mServo.GetAngle(), 90U );
	EXPECT_FALSE( mServo.IsMoving() );
}

TEST_F( UT_CMP_SERVO, SetAngle_BelowMin_WithOffset_Ko )
{
	mServo.SetOffset( 10 );
	mServo.SetMin( 50U );
	mServo.Initialize();

	EXPECT_FALSE( mServo.SetAngle( 50U ) );
	EXPECT_EQ( mServo.GetAngle(), 90U );
	EXPECT_FALSE( mServo.IsMoving() );
}

TEST_F( UT_CMP_SERVO, SetMin_Ok )
{
	mServo.Initialize();
	EXPECT_TRUE( mServo.SetMin( 50U ) );
	EXPECT_EQ( mServo.GetMin(), 50U );
}

TEST_F( UT_CMP_SERVO, SetMax_Ok )
{
	mServo.Initialize();

	EXPECT_TRUE( mServo.SetMax( 90U ) );
	EXPECT_EQ( mServo.GetMax(), 90U );
}
