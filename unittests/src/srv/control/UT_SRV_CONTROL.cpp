#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServos.h"

#include "../../../../src/clu/ClusterServo.h"
#include "../../../../src/srv/ServiceControl.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

class UT_SRV_CONTROL : public ::testing::Test  {
protected:
	UT_SRV_CONTROL() :
		mMockPca9685(),
		mMockServos(),
		mClusterServo( mMockServos ),
		mServiceControl( mClusterServo )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_CONTROL() = default;

	/* Mocks */
	StrictMock <MockPca9685> mMockPca9685;
	StrictMock <MockServos> mMockServos;

	ClusterServo mClusterServo;

	/* Test class */
	ServiceControl mServiceControl;
};

TEST_F( UT_SRV_CONTROL, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockServos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceControl.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_CONTROL, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockServos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceControl.Initialize();

	EXPECT_CALL( mMockServos, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServos, GetPca9685( _ ) ).WillOnce( ReturnRef( mMockPca9685 ) );
	EXPECT_CALL( mMockPca9685, Update( _ ) ).Times( 1U );

	mServiceControl.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_CONTROL, Update_2Times_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockServos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceControl.Initialize();

	EXPECT_CALL( mMockServos, Update( _ ) ).Times( 2U );
	EXPECT_CALL( mMockServos, GetPca9685( 0 ) ).WillOnce( ReturnRef( mMockPca9685 ) );
	EXPECT_CALL( mMockServos, GetPca9685( 1 ) ).WillOnce( ReturnRef( mMockPca9685 ) );
	EXPECT_CALL( mMockPca9685, Update( _ ) ).Times( 2U );

	mServiceControl.Update( 0UL );
	mServiceControl.Update( 0UL );

	EXPECT_TRUE( success );
}
