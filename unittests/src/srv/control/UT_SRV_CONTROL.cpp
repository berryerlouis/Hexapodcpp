#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServos.h"
#include "../../../mock/cor/MockEventMediatorInterface.h"

#include "../../../../src/Service/Control/ServiceControl.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace Control {
class UT_SRV_CONTROL : public ::testing::Test {
protected:
	UT_SRV_CONTROL() :
		mMockPca9685(),
		mMockServos(),
		mMockEventMediatorInterface(),
		mServiceControl( mMockServos )
	{
	}

	virtual void SetUp ()
	{
		EXPECT_CALL( mMockServos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );
		EXPECT_FALSE( mServiceControl.Initialize() );

		EXPECT_CALL( mMockServos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_TRUE( mServiceControl.Initialize() );
		mServiceControl.setMediator( &mMockEventMediatorInterface );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_CONTROL() = default;

	/* Mocks */
	StrictMock <Component::ServosController::MockPca9685> mMockPca9685;
	StrictMock <Component::Servos::MockServos> mMockServos;
	StrictMock <Core::MockEventMediatorInterface> mMockEventMediatorInterface;

	/* Test class */
	ServiceControl mServiceControl;
};

TEST_F( UT_SRV_CONTROL, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockServos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceControl.Initialize();

	EXPECT_CALL( mMockServos, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServos, GetServosController( _ ) ).WillOnce( ReturnRef( mMockPca9685 ) );
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
	EXPECT_CALL( mMockServos, GetServosController( 0 ) ).WillOnce( ReturnRef( mMockPca9685 ) );
	EXPECT_CALL( mMockServos, GetServosController( 1 ) ).WillOnce( ReturnRef( mMockPca9685 ) );
	EXPECT_CALL( mMockPca9685, Update( _ ) ).Times( 2U );

	mServiceControl.Update( 0UL );
	mServiceControl.Update( 0UL );

	EXPECT_TRUE( success );
}
}
}
