#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSoftware.h"
#include "../../../mock/cor/MockEventMediatorInterface.h"

#include "../../../../src/Service/General/ServiceGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace General {
class UT_SRV_GENERAL : public ::testing::Test {
protected:
	UT_SRV_GENERAL() :
		mMockSoftware(),
		mMockEventMediatorInterface(),
		mServiceGeneral( mMockSoftware )
	{
	}

	virtual void SetUp ()
	{
		EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );
		EXPECT_FALSE( mServiceGeneral.Initialize() );
		EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

		EXPECT_TRUE( mServiceGeneral.Initialize() );
		mServiceGeneral.setMediator( &mMockEventMediatorInterface );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_GENERAL() = default;

	/* Mocks */
	StrictMock <Component::Software::MockSoftware> mMockSoftware;
	StrictMock <Core::MockEventMediatorInterface> mMockEventMediatorInterface;

	/* Test class */
	ServiceGeneral mServiceGeneral;
};


TEST_F( UT_SRV_GENERAL, Update_FirstTimeUpdate_Ok )
{
	EXPECT_CALL( mMockSoftware, GetMinTime() ).Times( 1U ).WillOnce( Return( 0UL ) );
	EXPECT_CALL( mMockSoftware, GetMaxTime() ).Times( 1U ).WillOnce( Return( 0UL ) );

	mServiceGeneral.SetNewUpdateTime( 1000UL );
	mServiceGeneral.Update( 1000UL );
}

TEST_F( UT_SRV_GENERAL, Update_MultipleUpdateSetMin_Ok )
{
	mServiceGeneral.SetNewUpdateTime( 1UL );
	for ( uint64_t i = 10UL; i > 1U; i-- )
	{
		EXPECT_CALL( mMockSoftware, GetMinTime() ).Times( 1U ).WillRepeatedly( Return( i + 1U ) );
		EXPECT_CALL( mMockSoftware, SetMinTime( _ ) ).Times( 1U );
		EXPECT_CALL( mMockEventMediatorInterface, SendMessage( _ ) ).Times( 1U );

		mServiceGeneral.Update( i );
		mServiceGeneral.SetNewUpdateTime( i - 1U );
	}
}

TEST_F( UT_SRV_GENERAL, Update_MultipleUpdateSetMax_Ok )
{
	mServiceGeneral.SetNewUpdateTime( 1UL );

	for ( uint64_t i = 2U; i < 10UL; i++ )
	{
		EXPECT_CALL( mMockSoftware, GetMinTime() ).Times( 1U ).WillOnce( Return( 0U ) );
		EXPECT_CALL( mMockSoftware, GetMaxTime() ).Times( 1U ).WillRepeatedly( Return( 0U ) );
		EXPECT_CALL( mMockSoftware, SetMaxTime( 1U ) ).Times( 1U );
		EXPECT_CALL( mMockEventMediatorInterface, SendMessage( _ ) ).Times( 1U );

		mServiceGeneral.Update( i );
		mServiceGeneral.SetNewUpdateTime( i );
	}
}
}
}
