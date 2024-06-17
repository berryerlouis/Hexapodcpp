#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServos.h"

#include "../../../../src/srv/ServiceControl.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;


TEST(ServiceControl, Initialize_Ok)
{
	Core::CoreStatus        success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockServos> servos;

	ServiceControl serviceControl(servos);

	EXPECT_CALL(servos, Initialize() ).WillOnce(Return(Core::CoreStatus::CORE_OK) );

	success = serviceControl.Initialize();

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}


TEST(ServiceControl, Update_Ok)
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockServos>  servos;
	StrictMock <MockPca9685> pca9685;

	ServiceControl serviceControl(servos);

	EXPECT_CALL(servos, Initialize() ).WillOnce(Return(Core::CoreStatus::CORE_OK) );

	success = serviceControl.Initialize();

	EXPECT_CALL(servos, Update(_) ).Times(1U);
	EXPECT_CALL(servos, GetPca9685(_) ).WillOnce(ReturnRef(pca9685) );
	EXPECT_CALL(pca9685, Update(_) ).Times(1U);

	serviceControl.Update(0UL);

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}



TEST(ServiceControl, Update_2Times_Ok)
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockServos>  servos;
	StrictMock <MockPca9685> pca9685;

	ServiceControl serviceControl(servos);

	EXPECT_CALL(servos, Initialize() ).WillOnce(Return(Core::CoreStatus::CORE_OK) );

	success = serviceControl.Initialize();

	EXPECT_CALL(servos, Update(_) ).Times(2U);
	EXPECT_CALL(servos, GetPca9685(0) ).WillOnce(ReturnRef(pca9685) );
	EXPECT_CALL(servos, GetPca9685(1) ).WillOnce(ReturnRef(pca9685) );
	EXPECT_CALL(pca9685, Update(_) ).Times(2U);

	serviceControl.Update(0UL);
	serviceControl.Update(0UL);

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}
