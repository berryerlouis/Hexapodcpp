#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockSoftware.h"

#include "../../../../src/Cluster/General/ClusterGeneral.h"
#include "../../../../src/Service/General/ServiceGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace General {
class UT_SRV_GENERAL : public ::testing::Test {
protected:
	UT_SRV_GENERAL() :
		mClusterGeneral( mMockSoftware ),
		mServiceGeneral( mClusterGeneral, mMockSoftware )
	{
	}

	virtual void SetUp ()
	{
		mServiceGeneral.SetComComponent( &mMockServiceMediator );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_GENERAL() = default;

	/* Mocks */
	StrictMock <Component::Software::MockSoftware> mMockSoftware;
	StrictMock <MockServiceMediator> mMockServiceMediator;

	ClusterGeneral mClusterGeneral;

	/* Test class */
	ServiceGeneral mServiceGeneral;
};

TEST_F( UT_SRV_GENERAL, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceGeneral.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_GENERAL, Initialize_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );

	success = mServiceGeneral.Initialize();

	EXPECT_FALSE( success );
}

TEST_F( UT_SRV_GENERAL, Update_FirstTimeUpdate_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mServiceGeneral.Initialize();

	EXPECT_CALL( mMockSoftware, GetMinTime() ).Times( 1U ).WillOnce( Return( 0UL ) );
	EXPECT_CALL( mMockSoftware, GetMaxTime() ).Times( 1U ).WillOnce( Return( 0UL ) );

	mServiceGeneral.SetNewUpdateTime( 1000UL );
	mServiceGeneral.Update( 1000UL );
	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_GENERAL, Update_MultipleUpdateSetMin_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mServiceGeneral.Initialize();

	mServiceGeneral.SetNewUpdateTime( 1UL );
	for ( uint64_t i = 10UL; i > 1U; i-- )
	{
		EXPECT_CALL( mMockSoftware, GetMinTime() ).Times( 2UL ).WillRepeatedly( Return( i + 1U ) );
		EXPECT_CALL( mMockSoftware, SetMinTime( _ ) ).Times( 1U );
		EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );

		mServiceGeneral.Update( i );
		mServiceGeneral.SetNewUpdateTime( i - 1U );
	}

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_GENERAL, Update_MultipleUpdateSetMax_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mServiceGeneral.Initialize();

	mServiceGeneral.SetNewUpdateTime( 1UL );

	for ( uint64_t i = 2U; i < 10UL; i++ )
	{
		EXPECT_CALL( mMockSoftware, GetMinTime() ).Times( 1UL ).WillOnce( Return( 0U ) );
		EXPECT_CALL( mMockSoftware, GetMaxTime() ).Times( 2UL ).WillRepeatedly( Return( 0U ) );
		EXPECT_CALL( mMockSoftware, SetMaxTime( 1U ) ).Times( 1U );
		EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );

		mServiceGeneral.Update( i );
		mServiceGeneral.SetNewUpdateTime( i );
	}

	EXPECT_TRUE( success );
}
}
}
