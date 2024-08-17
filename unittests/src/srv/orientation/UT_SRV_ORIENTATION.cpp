#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockMpu9150.h"


#include "../../../../src/clu/ClusterImu.h"
#include "../../../../src/srv/ServiceOrientation.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

class UT_SRV_ORIENTATION : public ::testing::Test  {
protected:
	UT_SRV_ORIENTATION() :
		mMockMpu9150(),
		mClusterImu( mMockMpu9150 ),
		mServiceOrientation( mClusterImu, mMockMpu9150 )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_ORIENTATION() = default;

	/* Mocks */
	StrictMock <MockMpu9150> mMockMpu9150;

	ClusterImu mClusterImu;

	/* Test class */
	ServiceOrientation mServiceOrientation;
};

TEST_F( UT_SRV_ORIENTATION, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockMpu9150, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceOrientation.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_ORIENTATION, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockMpu9150, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceOrientation.Initialize();

	EXPECT_CALL( mMockMpu9150, Update( _ ) ).Times( 1U );

	mServiceOrientation.Update( 0UL );

	EXPECT_TRUE( success );
}
