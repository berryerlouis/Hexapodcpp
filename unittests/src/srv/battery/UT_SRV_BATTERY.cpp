#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockBattery.h"

#include "../../../../src/clu/ClusterBattery.h"
#include "../../../../src/srv/ServiceBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST( ServiceBattery, Initialize_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	ServiceBattery serviceBattery( clusterBattery );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = serviceBattery.Initialize();

	EXPECT_TRUE( success );
}

TEST( ServiceBattery, Initialize_Ko )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	ServiceBattery serviceBattery( clusterBattery );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );

	success = serviceBattery.Initialize();

	EXPECT_FALSE( success );
}

TEST( ServiceBattery, Update_FirstTimeUpdate_Ok )
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockBattery>         battery;
	ClusterBattery                   clusterBattery( battery );
	StrictMock <MockServiceMediator> mediator;

	ServiceBattery serviceBattery( clusterBattery );
	serviceBattery.SetComComponent( &mediator );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = serviceBattery.Initialize();

	EXPECT_CALL( battery, Update( _ ) ).Times( 1U );
	EXPECT_CALL( battery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryInterface::BatteryState::NOMINAL ) );
	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mediator, DisplayBatteryLevel( BatteryInterface::BatteryState::NOMINAL ) ).Times( 1U );

	serviceBattery.Update( 0UL );
	EXPECT_TRUE( success );
}

TEST( ServiceBattery, Update_SentFrameOnceAtFirstUpdate_Ok )
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockBattery>         battery;
	ClusterBattery                   clusterBattery( battery );
	StrictMock <MockServiceMediator> mediator;
	uint8_t nbUpdate = 5U;

	ServiceBattery serviceBattery( clusterBattery );
	serviceBattery.SetComComponent( &mediator );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = serviceBattery.Initialize();

	EXPECT_CALL( battery, Update( _ ) ).Times( nbUpdate );
	EXPECT_CALL( battery, GetState() ).Times( nbUpdate + 1U ).WillRepeatedly( Return( BatteryInterface::BatteryState::NOMINAL ) );
	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mediator, DisplayBatteryLevel( BatteryInterface::BatteryState::NOMINAL ) ).Times( 1U );

	for ( size_t i = 0; i < nbUpdate; i++ )
	{
		serviceBattery.Update( 0UL );
	}

	EXPECT_TRUE( success );
}

TEST( ServiceBattery, Update_SentFrameOnceAtFirstUpdateAndOnceAfterStateChanged_Ok )
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockBattery>         battery;
	ClusterBattery                   clusterBattery( battery );
	StrictMock <MockServiceMediator> mediator;
	uint8_t nbUpdate = 5U;

	ServiceBattery serviceBattery( clusterBattery );
	serviceBattery.SetComComponent( &mediator );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = serviceBattery.Initialize();


	EXPECT_CALL( battery, Update( 0UL ) ).Times( 1U );
	EXPECT_CALL( battery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryInterface::BatteryState::NOMINAL ) );
	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mediator, DisplayBatteryLevel( BatteryInterface::BatteryState::NOMINAL ) ).Times( 1U );
	serviceBattery.Update( 0UL );

	EXPECT_CALL( battery, Update( 1UL ) ).Times( 1U );
	EXPECT_CALL( battery, GetState() ).Times( 1U ).WillOnce( Return( BatteryInterface::BatteryState::NOMINAL ) );
	serviceBattery.Update( 1UL );

	EXPECT_CALL( battery, Update( 2UL ) ).Times( 1U );
	EXPECT_CALL( battery, GetState() ).Times( 2U ).WillRepeatedly( Return( BatteryInterface::BatteryState::CRITICAL ) );
	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mediator, DisplayBatteryLevel( BatteryInterface::BatteryState::CRITICAL ) ).Times( 1U );
	serviceBattery.Update( 2UL );

	EXPECT_CALL( battery, Update( 3UL ) ).Times( 1U );
	EXPECT_CALL( battery, GetState() ).Times( 1U ).WillOnce( Return( BatteryInterface::BatteryState::CRITICAL ) );
	serviceBattery.Update( 3UL );

	EXPECT_TRUE( success );
}
