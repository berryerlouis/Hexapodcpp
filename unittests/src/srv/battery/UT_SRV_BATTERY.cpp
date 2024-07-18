#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockBattery.h"

#include "../../../../src/srv/ServiceBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

class UT_SRV_BATTERY : public ::testing::Test  {
protected:
	UT_SRV_BATTERY() :
		mMockBattery(),
		mClusterBattery( mMockBattery ),
		mMockServiceMediator(),
		mServiceBattery( mClusterBattery )
	{
	}

	virtual void SetUp ()
	{
		mServiceBattery.SetComComponent( &mMockServiceMediator );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_BATTERY() = default;

	/* Mocks */
	StrictMock <MockBattery> mMockBattery;
	StrictMock <MockServiceMediator> mMockServiceMediator;

	ClusterBattery mClusterBattery;

	/* Test class */
	ServiceBattery mServiceBattery;
};

TEST_F( UT_SRV_BATTERY, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceBattery.Initialize();

	EXPECT_TRUE( success );
}


TEST_F( UT_SRV_BATTERY, Initialize_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );

	success = mServiceBattery.Initialize();

	EXPECT_FALSE( success );
}

TEST_F( UT_SRV_BATTERY, Update_FirstTimeUpdate_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mServiceBattery.Initialize();

	EXPECT_CALL( mMockBattery, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockBattery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryInterface::BatteryState::NOMINAL ) );
	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryInterface::BatteryState::NOMINAL ) ).Times( 1U );

	mServiceBattery.Update( 0UL );
	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_BATTERY, Update_SentFrameOnceAtFirstUpdate_Ok )
{
	Core::CoreStatus success  = Core::CoreStatus::CORE_ERROR;
	uint8_t          nbUpdate = 5U;

	EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mServiceBattery.Initialize();

	EXPECT_CALL( mMockBattery, Update( _ ) ).Times( nbUpdate );
	EXPECT_CALL( mMockBattery, GetState() ).Times( nbUpdate + 1U ).WillRepeatedly( Return( BatteryInterface::BatteryState::NOMINAL ) );
	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryInterface::BatteryState::NOMINAL ) ).Times( 1U );

	for ( size_t i = 0U; i < nbUpdate; i++ )
	{
		mServiceBattery.Update( 0UL );
	}

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_BATTERY, Update_SentFrameOnceAtFirstUpdateAndOnceAfterStateChanged_Ok )
{
	Core::CoreStatus success  = Core::CoreStatus::CORE_ERROR;
	uint8_t          nbUpdate = 5U;

	EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mServiceBattery.Initialize();

	EXPECT_CALL( mMockBattery, Update( 0UL ) ).Times( 1U );
	EXPECT_CALL( mMockBattery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryInterface::BatteryState::NOMINAL ) );
	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryInterface::BatteryState::NOMINAL ) ).Times( 1U );
	mServiceBattery.Update( 0UL );

	EXPECT_CALL( mMockBattery, Update( 1UL ) ).Times( 1U );
	EXPECT_CALL( mMockBattery, GetState() ).Times( 1U ).WillOnce( Return( BatteryInterface::BatteryState::NOMINAL ) );
	mServiceBattery.Update( 1UL );

	EXPECT_CALL( mMockBattery, Update( 2UL ) ).Times( 1U );
	EXPECT_CALL( mMockBattery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryInterface::BatteryState::CRITICAL ) );
	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryInterface::BatteryState::CRITICAL ) ).Times( 1U );
	mServiceBattery.Update( 2UL );

	EXPECT_CALL( mMockBattery, Update( 3UL ) ).Times( 1U );
	EXPECT_CALL( mMockBattery, GetState() ).Times( 1U ).WillOnce( Return( BatteryInterface::BatteryState::CRITICAL ) );
	mServiceBattery.Update( 3UL );

	EXPECT_TRUE( success );
}
