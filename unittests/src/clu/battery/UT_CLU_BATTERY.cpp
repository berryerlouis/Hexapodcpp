#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockBattery.h"
#include "../../../../src/clu/ClusterBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

TEST( ClusterBattery, Execute_WrongCluster_Ko )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          request;
	Clusters::Frame          response;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = clusterBattery.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterBattery, Execute_WrongCommand_Ko )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          request;
	Clusters::Frame          response;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	request.Build( Clusters::EClusters::BATTERY, 0x5FU );
	success = clusterBattery.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterBattery, Execute_GET_VOLTAGE_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          request;
	Clusters::Frame          response;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	request.Build( Clusters::EClusters::BATTERY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = clusterBattery.Execute( request, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST( ClusterBattery, Execute_GET_BAT_STATUS_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          request;
	Clusters::Frame          response;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	EXPECT_CALL( battery, GetState() ).WillOnce( Return( BatteryInterface::BatteryState::NOMINAL ) );
	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	request.Build( Clusters::EClusters::BATTERY, Clusters::EBatteryCommands::GET_BAT_STATUS );
	success = clusterBattery.Execute( request, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_BAT_STATUS );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], BatteryInterface::BatteryState::NOMINAL );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_TRUE( success );
}


TEST( ClusterBattery, BuildFrameState_Nominal_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterBattery.Initialize();

	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( battery, GetState() ).WillOnce( Return( BatteryInterface::BatteryState::NOMINAL ) );

	clusterBattery.BuildFrameState( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_BAT_STATUS );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], 0U );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_TRUE( success );
}

TEST( ClusterBattery, BuildFrameState_Critical_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterBattery.Initialize();

	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( battery, GetState() ).WillOnce( Return( BatteryInterface::BatteryState::CRITICAL ) );

	clusterBattery.BuildFrameState( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_BAT_STATUS );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], 2U );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_TRUE( success );
}

TEST( ClusterBattery, BuildFrameState_Warning_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	StrictMock <MockBattery> battery;
	ClusterBattery           clusterBattery( battery );

	EXPECT_CALL( battery, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterBattery.Initialize();

	EXPECT_CALL( battery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( battery, GetState() ).WillOnce( Return( BatteryInterface::BatteryState::WARNING ) );

	clusterBattery.BuildFrameState( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::BATTERY );
	EXPECT_EQ( response.commandId, Clusters::EBatteryCommands::GET_BAT_STATUS );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], 1U );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_TRUE( success );
}
