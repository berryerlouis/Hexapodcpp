#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockAdc.h"
#include "../../../../src/cmp/Battery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

class UT_CMP_BATTERY : public ::testing::Test {
protected:
	UT_CMP_BATTERY() :
		mMockAdc(),
		mBattery( mMockAdc )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_BATTERY() = default;

	/* Mocks */
	StrictMock <MockAdc> mMockAdc;

	/* Test class */
	Battery mBattery;
};

TEST_F( UT_CMP_BATTERY, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockAdc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mBattery.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_BATTERY, GetStateAfterInit )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockAdc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mBattery.Initialize();
	BatteryState state = mBattery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, BatteryState::UNKNOWN );
}


TEST_F( UT_CMP_BATTERY, GetStateAfterUpdateCritical )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockAdc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockAdc, Read() ).WillOnce( Return( 74U ) );

	success = mBattery.Initialize();
	mBattery.Update( 0UL );
	BatteryState state = mBattery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, BatteryState::CRITICAL );
}

TEST_F( UT_CMP_BATTERY, GetStateAfterUpdateWarning )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockAdc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockAdc, Read() ).WillOnce( Return( 79U ) );

	success = mBattery.Initialize();
	mBattery.Update( 0UL );
	BatteryState state = mBattery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, BatteryState::WARNING );
}

TEST_F( UT_CMP_BATTERY, GetStateAfterUpdateNominal )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockAdc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockAdc, Read() ).WillOnce( Return( 90U ) );

	success = mBattery.Initialize();
	mBattery.Update( 0UL );
	BatteryState state = mBattery.GetState();

	EXPECT_TRUE( success );
	EXPECT_EQ( state, BatteryState::NOMINAL );
}
