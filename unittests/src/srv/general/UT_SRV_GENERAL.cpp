#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockSoftware.h"

#include "../../../../src/clu/ClusterGeneral.h"
#include "../../../../src/srv/ServiceGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST( ServiceGeneral, Initialize_Ok )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	ServiceGeneral serviceGeneral( clusterGeneral );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = serviceGeneral.Initialize();

	EXPECT_TRUE( success );
}

TEST( ServiceGeneral, Initialize_Ko )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	ServiceGeneral serviceGeneral( clusterGeneral );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );

	success = serviceGeneral.Initialize();

	EXPECT_FALSE( success );
}

TEST( ServiceGeneral, Update_FirstTimeUpdate_Ok )
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSoftware>        software;
	ClusterGeneral                   clusterGeneral( software );
	StrictMock <MockServiceMediator> mediator;

	ServiceGeneral serviceGeneral( clusterGeneral );
	serviceGeneral.SetComComponent( &mediator );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = serviceGeneral.Initialize();

	EXPECT_CALL( software, GetMinTime() ).Times( 1U ).WillOnce( Return( 0UL ) );
	EXPECT_CALL( software, GetMaxTime() ).Times( 1U ).WillOnce( Return( 0UL ) );

	serviceGeneral.Update( 0UL );
	EXPECT_TRUE( success );
}


TEST( ServiceGeneral, Update_MultipleUpdateSetMin_Ok )
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSoftware>        software;
	ClusterGeneral                   clusterGeneral( software );
	StrictMock <MockServiceMediator> mediator;

	ServiceGeneral serviceGeneral( clusterGeneral );
	serviceGeneral.SetComComponent( &mediator );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = serviceGeneral.Initialize();

	for ( size_t i = 10UL; i > 1U; i-- )
	{
		EXPECT_CALL( software, GetMinTime() ).Times( 2UL ).WillRepeatedly( Return( i + 1U ) );
		EXPECT_CALL( software, SetMinTime( i ) ).Times( 1U );
		EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );

		serviceGeneral.Update( i );
	}

	EXPECT_TRUE( success );
}

TEST( ServiceGeneral, Update_MultipleUpdateSetMax_Ok )
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSoftware>        software;
	ClusterGeneral                   clusterGeneral( software );
	StrictMock <MockServiceMediator> mediator;

	ServiceGeneral serviceGeneral( clusterGeneral );
	serviceGeneral.SetComComponent( &mediator );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = serviceGeneral.Initialize();

	for ( size_t i = 1; i < 10UL; i++ )
	{
		EXPECT_CALL( software, GetMinTime() ).Times( 1UL ).WillOnce( Return( i - 1U ) );
		EXPECT_CALL( software, GetMaxTime() ).Times( 2UL ).WillRepeatedly( Return( i - 1U ) );
		EXPECT_CALL( software, SetMaxTime( i ) ).Times( 1U );
		EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );

		serviceGeneral.Update( i );
	}

	EXPECT_TRUE( success );
}
