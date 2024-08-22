#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/Component/Servos/Servos.h"

using ::testing::StrictMock;
using ::testing::_;
using ::testing::Return;

namespace Component {
namespace Servos {
class UT_CMP_SERVOS : public ::testing::Test {
protected:
	UT_CMP_SERVOS() :
		mMockTick(),
		mMockPca9685_0(),
		mMockPca9685_1(),
		mServos( mMockPca9685_0, mMockPca9685_1, mMockTick )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_SERVOS() = default;

	/* Mocks */
	StrictMock <Driver::Tick::MockTick> mMockTick;
	StrictMock <Component::ServosController::MockPca9685> mMockPca9685_0;
	StrictMock <Component::ServosController::MockPca9685> mMockPca9685_1;

	/* Test class */
	Servos mServos;
};

TEST_F( UT_CMP_SERVOS, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockPca9685_0, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockPca9685_1, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServos.Initialize();
	EXPECT_TRUE( success );
}


TEST_F( UT_CMP_SERVOS, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockPca9685_0, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockPca9685_1, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServos.Initialize();

	EXPECT_CALL( mMockPca9685_0, SetPwm( _, _ ) ).Times( ServosInterface::NB_SERVOS / 2U );
	EXPECT_CALL( mMockPca9685_1, SetPwm( _, _ ) ).Times( ServosInterface::NB_SERVOS / 2U );

	mServos.Update( 0UL );

	EXPECT_TRUE( success );
}
}
}
