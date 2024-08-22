#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockSensorProximity.h"

#include "../../../../src/clu/ClusterProximity.h"
#include "../../../../src/srv/ServiceProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

class UT_SRV_PROXIMITY : public ::testing::Test {
protected:
	UT_SRV_PROXIMITY() :
		mMockSensorProximity(),
		mMockServiceMediator(),
		mClusterProximity( mMockSensorProximity ),
		mServiceProximity( mClusterProximity, mMockSensorProximity )
	{
	}

	virtual void SetUp ()
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		EXPECT_CALL( mMockSensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

		success = mServiceProximity.Initialize();
		mServiceProximity.SetComComponent( &mMockServiceMediator );

		EXPECT_TRUE( success );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_PROXIMITY() = default;

	/* Mocks */
	StrictMock <MockSensorProximity> mMockSensorProximity;
	StrictMock <MockServiceMediator> mMockServiceMediator;

	ClusterProximity mClusterProximity;

	/* Test class */
	ServiceProximity mServiceProximity;
};

TEST_F( UT_SRV_PROXIMITY, Update_NoDetection_Ok )
{
	EXPECT_CALL( mMockSensorProximity, Update( _ ) ).Times( 1U );

	mServiceProximity.Update( 0UL );
}

TEST_F( UT_SRV_PROXIMITY, Update_Detection_Srf_Ok )
{
	EXPECT_CALL( mMockSensorProximity, Update( _ ) ).Times( 1U );

	mServiceProximity.Update( 0UL );
}
