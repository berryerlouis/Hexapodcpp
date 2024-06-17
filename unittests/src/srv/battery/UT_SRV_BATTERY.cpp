#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockBattery.h"

#include "../../../../src/srv/ServiceBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST(ServiceBattery, Initialize_Ok)
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockBattery> battery;

	ServiceBattery serviceBattery(battery);

	EXPECT_CALL(battery, Initialize() ).WillOnce(Return(Core::CoreStatus::CORE_OK) );

	success = serviceBattery.Initialize();

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}

TEST(ServiceBattery, Update_Ok)
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockBattery>         battery;
	StrictMock <MockServiceMediator> mediator;

	ServiceBattery serviceBattery(battery);
	serviceBattery.SetComComponent(&mediator);

	EXPECT_CALL(battery, Initialize() ).WillOnce(Return(Core::CoreStatus::CORE_OK) );
	success = serviceBattery.Initialize();

	EXPECT_CALL(battery, Update(_) ).Times(1U);
	EXPECT_CALL(battery, GetState() ).WillOnce(Return(BatteryInterface::BatteryState::NOMINAL) );
	EXPECT_CALL(battery, GetVoltage() ).WillOnce(Return(10U) );
	EXPECT_CALL(mediator, SendFrame(_) ).Times(1U);

	serviceBattery.Update(0UL);
	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}

TEST(ServiceBattery, BuildFrameDistance_Ok)
{
	Core::CoreStatus                 success = Core::CoreStatus::CORE_ERROR;
	Cluster::Frame                   response;
	StrictMock <MockBattery>         battery;
	StrictMock <MockServiceMediator> mediator;

	ServiceBattery serviceBattery(battery);
	serviceBattery.SetComComponent(&mediator);

	EXPECT_CALL(battery, Initialize() ).WillOnce(Return(Core::CoreStatus::CORE_OK) );
	success = serviceBattery.Initialize();

	EXPECT_CALL(battery, GetVoltage() ).WillOnce(Return(10U) );

	serviceBattery.BuildFrameState(response);
	EXPECT_EQ(response.clusterId, Cluster::EClusters::BATTERY);
	EXPECT_EQ(response.commandId, Cluster::EBatteryCommands::GET_BAT_STATUS);
	EXPECT_EQ(response.nbParams, 3U);
	EXPECT_EQ(response.params[0U], 0xFFU);
	EXPECT_EQ(response.params[1U], 0U);
	EXPECT_EQ(response.params[2U], 10U);
	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}
