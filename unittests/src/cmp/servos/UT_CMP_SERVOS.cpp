#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/cmp/Servos.h"

using ::testing::StrictMock;
using ::testing::_;
using ::testing::Return;

using namespace Component;

TEST( ComponentServos, Initialize_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685_0;
	StrictMock <MockPca9685> pca9685_1;

	Servos servos( pca9685_0, pca9685_1, tick );
	EXPECT_CALL( pca9685_0, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( pca9685_1, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = servos.Initialize();

	EXPECT_TRUE( success );
}


TEST( ComponentServos, Update_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685_0;
	StrictMock <MockPca9685> pca9685_1;

	Servos servos( pca9685_0, pca9685_1, tick );
	EXPECT_CALL( pca9685_0, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( pca9685_1, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( pca9685_0, SetPwm( _, _ ) ).Times( ServosInterface::NB_SERVOS / 2U );
	EXPECT_CALL( pca9685_1, SetPwm( _, _ ) ).Times( ServosInterface::NB_SERVOS / 2U );

	success = servos.Initialize();

	servos.Update( 0UL );

	EXPECT_TRUE( success );
}
