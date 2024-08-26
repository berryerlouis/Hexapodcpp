#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/cmp/MockMpu9150.h"
#include "../../../mock/cor/MockEventMediatorInterface.h"

#include "../../../../src/Service/Orientation/ServiceOrientation.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace Orientation {
class UT_SRV_ORIENTATION : public ::testing::Test {
protected:
	UT_SRV_ORIENTATION() :
		mMockMpu9150(),
		mMockEventMediatorInterface(),
		mServiceOrientation( mMockMpu9150 )
	{
	}

	virtual void SetUp ()
	{
		EXPECT_CALL( mMockMpu9150, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );
		EXPECT_FALSE( mServiceOrientation.Initialize() );

		EXPECT_CALL( mMockMpu9150, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_TRUE( mServiceOrientation.Initialize() );
		mServiceOrientation.setMediator( &mMockEventMediatorInterface );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_ORIENTATION() = default;

	/* Mocks */
	StrictMock <Component::Imu::MockMpu9150> mMockMpu9150;
	StrictMock <Core::MockEventMediatorInterface> mMockEventMediatorInterface;

	/* Test class */
	ServiceOrientation mServiceOrientation;
};

TEST_F( UT_SRV_ORIENTATION, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockMpu9150, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceOrientation.Initialize();

	EXPECT_CALL( mMockMpu9150, Update( _ ) ).Times( 1U );

	mServiceOrientation.Update( 0UL );

	EXPECT_TRUE( success );
}
}
}
