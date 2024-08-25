#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockAdc.h"
#include "../../../mock/srv/MockServiceMediator.h"

#include "../../../../src/Service/Battery/ServiceBattery.h"
#include "../../../../src/Component/Battery/Battery.h"
#include "../../../../src/Cluster/Battery/ClusterBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace Battery {
class UT_SRV_BATTERY : public ::testing::Test {
protected:
	UT_SRV_BATTERY() :
		mMockAdc(),
		mBattery( mMockAdc ),
		mMockServiceMediator(),
		mClusterBattery( mBattery ),
		mServiceBattery( mClusterBattery, mBattery )
	{
	}

	virtual void SetUp ()
	{
		EXPECT_CALL( mMockAdc, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_TRUE( mServiceBattery.Initialize() );

		mServiceBattery.SetEventManager( &mMockServiceMediator );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_BATTERY() = default;

	/* Class */
	Component::Battery::Battery mBattery;
	Cluster::Battery::ClusterBattery mClusterBattery;

	/* Mocks */
	StrictMock <Driver::Adc::MockAdc> mMockAdc;
	StrictMock <MockServiceMediator> mMockServiceMediator;

	/* Test class */
	ServiceBattery mServiceBattery;
};

TEST_F( UT_SRV_BATTERY, Initialize_NotifyNominal )
{
	EXPECT_CALL( mMockAdc, Read() ).WillOnce( Return( 90U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );

	mBattery.Update( 0UL );
}

TEST_F( UT_SRV_BATTERY, Initialize_NotifyNominalTwice )
{
	EXPECT_CALL( mMockAdc, Read() ).WillRepeatedly( Return( 90U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );

	mBattery.Update( 0UL );
	mBattery.Update( 0UL );
}

TEST_F( UT_SRV_BATTERY, Initialize_NotifyWarning )
{
	EXPECT_CALL( mMockAdc, Read() ).WillOnce( Return( 79U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );

	mBattery.Update( 0UL );
}

TEST_F( UT_SRV_BATTERY, Initialize_NotifyCritical )
{
	EXPECT_CALL( mMockAdc, Read() ).WillOnce( Return( 74U ) );
	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );

	mBattery.Update( 0UL );
}
}
}

/*
 *
 * TEST_F( UT_SRV_BATTERY, Update_FirstTimeUpdate_Ok )
 * {
 *      Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
 *
 *      EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
 *      success = mServiceBattery.Initialize();
 *
 *      EXPECT_CALL( mMockBattery, Update( _ ) ).Times( 1U );
 *      EXPECT_CALL( mMockBattery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryState::NOMINAL ) );
 *      EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
 *      EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
 *      EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryState::NOMINAL ) ).Times( 1U );
 *
 *      mServiceBattery.Update( 0UL );
 *      EXPECT_TRUE( success );
 * }
 *
 * TEST_F( UT_SRV_BATTERY, Update_SentFrameOnceAtFirstUpdate_Ok )
 * {
 *      Core::CoreStatus success  = Core::CoreStatus::CORE_ERROR;
 *      uint8_t          nbUpdate = 5U;
 *
 *      EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
 *      success = mServiceBattery.Initialize();
 *
 *      EXPECT_CALL( mMockBattery, Update( _ ) ).Times( nbUpdate );
 *      EXPECT_CALL( mMockBattery, GetState() ).Times( nbUpdate + 1U ).WillRepeatedly( Return( BatteryState::NOMINAL ) );
 *      EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
 *      EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
 *      EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryState::NOMINAL ) ).Times( 1U );
 *
 *      for ( size_t i = 0U; i < nbUpdate; i++ )
 *      {
 *              mServiceBattery.Update( 0UL );
 *      }
 *
 *      EXPECT_TRUE( success );
 * }
 *
 * TEST_F( UT_SRV_BATTERY, Update_SentFrameOnceAtFirstUpdateAndOnceAfterStateChanged_Ok )
 * {
 *      Core::CoreStatus success  = Core::CoreStatus::CORE_ERROR;
 *      uint8_t          nbUpdate = 5U;
 *
 *      EXPECT_CALL( mMockBattery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
 *      success = mServiceBattery.Initialize();
 *
 *      EXPECT_CALL( mMockBattery, Update( 0UL ) ).Times( 1U );
 *      EXPECT_CALL( mMockBattery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryState::NOMINAL ) );
 *      EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
 *      EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
 *      EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryState::NOMINAL ) ).Times( 1U );
 *      mServiceBattery.Update( 0UL );
 *
 *      EXPECT_CALL( mMockBattery, Update( 1UL ) ).Times( 1U );
 *      EXPECT_CALL( mMockBattery, GetState() ).Times( 1U ).WillOnce( Return( BatteryState::NOMINAL ) );
 *      mServiceBattery.Update( 1UL );
 *
 *      EXPECT_CALL( mMockBattery, Update( 2UL ) ).Times( 1U );
 *      EXPECT_CALL( mMockBattery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryState::CRITICAL ) );
 *      EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
 *      EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
 *      EXPECT_CALL( mMockServiceMediator, DisplayBatteryLevel( BatteryState::CRITICAL ) ).Times( 1U );
 *      mServiceBattery.Update( 2UL );
 *
 *      EXPECT_CALL( mMockBattery, Update( 3UL ) ).Times( 1U );
 *      EXPECT_CALL( mMockBattery, GetState() ).Times( 1U ).WillOnce( Return( BatteryState::CRITICAL ) );
 *      mServiceBattery.Update( 3UL );
 *
 *      EXPECT_TRUE( success );
 * }
 */
