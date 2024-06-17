#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/cmp/Servos.h"

using ::testing::StrictMock;
using ::testing::_;

using namespace Component;

TEST(ComponentServos, Initialize_Ok)
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685_0;
	StrictMock <MockPca9685> pca9685_1;

	Servos servos(pca9685_0, pca9685_1, tick);
	EXPECT_CALL(pca9685_0, Initialize() ).Times(1U);
	EXPECT_CALL(pca9685_1, Initialize() ).Times(1U);
	EXPECT_CALL(tick, GetMs() ).Times(NB_SERVOS);

	success = servos.Initialize();

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}


TEST(ComponentServos, Update_Ok)
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685_0;
	StrictMock <MockPca9685> pca9685_1;

	Servos servos(pca9685_0, pca9685_1, tick);
	EXPECT_CALL(pca9685_0, Initialize() ).Times(1U);
	EXPECT_CALL(pca9685_1, Initialize() ).Times(1U);
	EXPECT_CALL(tick, GetMs() ).Times(NB_SERVOS);
	EXPECT_CALL(pca9685_0, SetPwm(_, _) ).Times(NB_SERVOS / 2U);
	EXPECT_CALL(pca9685_1, SetPwm(_, _) ).Times(NB_SERVOS / 2U);

	success = servos.Initialize();

	servos.Update(0UL);

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}
