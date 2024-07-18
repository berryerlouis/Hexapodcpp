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

class UT_CMP_SRF05 : public ::testing::Test  {
protected:
	UT_CMP_SRF05() :
		mMockTick(),
		mMockGpio(),
		mMockInputCapture(),
		mMockLed(),
		mSrf05( EProximityCommands::US_LEFT, mMockGpio, mMockInputCapture, mMockLed, mMockTick )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_SRF05() = default;

	/* Mocks */
	StrictMock <MockTick> mMockTick;
	StrictMock <MockGpio> mMockGpio;
	StrictMock <MockInputCapture> mMockInputCapture;
	StrictMock <MockLed> mMockLed;

	/* Test class */
	Srf05 mSrf05;
};

TEST_F( UT_CMP_SRF05, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockInputCapture, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mSrf05.Initialize();

	EXPECT_EQ( mSrf05.GetThreshold(), 30U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_SRF05, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockInputCapture, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockGpio, Set() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockTick, DelayUs( _ ) ).Times( 1U );
	EXPECT_CALL( mMockGpio, Reset() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mSrf05.Initialize();
	mSrf05.Update( 0U );

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_SRF05, IsDetecting_SetThreshold )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockInputCapture, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mSrf05.Initialize();

	EXPECT_TRUE( mSrf05.SetThreshold( 35U ) );
	EXPECT_EQ( 35U, mSrf05.GetThreshold() );
	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_SRF05, IsDetecting_UnderThreshold )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockInputCapture, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockInputCapture, GetInputCaptureTime() ).Times( 1U ).WillOnce( Return( 580U ) );
	EXPECT_CALL( mMockLed, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, On ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mSrf05.Initialize();

	EXPECT_TRUE( mSrf05.IsDetecting() );
	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_SRF05, IsDetecting_AboveThreshold )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockInputCapture, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockInputCapture, GetInputCaptureTime() ).Times( 1U ).WillOnce( Return( 58U * 31U ) );
	EXPECT_CALL( mMockLed, Initialize() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Off() ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mSrf05.Initialize();

	EXPECT_FALSE( mSrf05.IsDetecting() );
	EXPECT_TRUE( success );
}
