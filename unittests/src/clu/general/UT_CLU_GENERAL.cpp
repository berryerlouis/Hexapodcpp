#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSoftware.h"
#include "../../../../src/clu/ClusterGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;


TEST( ClusterGeneral, Execute_WrongCluster_Ko )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame           request;
	Clusters::Frame           response;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = clusterGeneral.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterGeneral, Execute_WrongCommand_Ko )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame           request;
	Clusters::Frame           response;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	request.Build( Clusters::EClusters::GENERAL, 0x5FU );
	success = clusterGeneral.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterGeneral, Execute_VERSION_Ok )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame           request;
	Clusters::Frame           response;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	EXPECT_CALL( software, GetVersion() ).WillOnce( Return( SoftwareInterface::Version{ 1, 0 } ) );
	request.Build( Clusters::EClusters::GENERAL, Clusters::EGeneralCommands::VERSION );
	success = clusterGeneral.Execute( request, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::GENERAL );
	EXPECT_EQ( response.commandId, Clusters::EGeneralCommands::VERSION );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_TRUE( success );
}

TEST( ClusterGeneral, BuildFrameGetVersion_Ok )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame           response;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterGeneral.Initialize();

	EXPECT_CALL( software, GetVersion() ).WillOnce( Return( SoftwareInterface::Version{ 1, 0 } ) );

	clusterGeneral.BuildFrameGetVersion( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::GENERAL );
	EXPECT_EQ( response.commandId, Clusters::EGeneralCommands::VERSION );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 1U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST( ClusterGeneral, BuildFrameGetMinTime_Ok )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame           response;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterGeneral.Initialize();

	EXPECT_CALL( software, GetMinTime() ).WillOnce( Return( 50U ) );

	clusterGeneral.BuildFrameGetMinTime( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::GENERAL );
	EXPECT_EQ( response.commandId, Clusters::EGeneralCommands::MIN_EXECUTION_TIME );
	EXPECT_EQ( response.nbParams, 8U );
	EXPECT_EQ( response.params[0U], 50U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_EQ( response.params[3U], 0U );
	EXPECT_EQ( response.params[4U], 0U );
	EXPECT_EQ( response.params[5U], 0U );
	EXPECT_EQ( response.params[6U], 0U );
	EXPECT_EQ( response.params[7U], 0U );
	EXPECT_TRUE( success );
}

TEST( ClusterGeneral, BuildFrameGetMaxTime_Ok )
{
	Core::CoreStatus          success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame           response;
	StrictMock <MockSoftware> software;
	ClusterGeneral            clusterGeneral( software );

	EXPECT_CALL( software, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterGeneral.Initialize();

	EXPECT_CALL( software, GetMaxTime() ).WillOnce( Return( 50U ) );

	clusterGeneral.BuildFrameGetMaxTime( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::GENERAL );
	EXPECT_EQ( response.commandId, Clusters::EGeneralCommands::MAX_EXECUTION_TIME );
	EXPECT_EQ( response.nbParams, 8U );
	EXPECT_EQ( response.params[0U], 50U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_EQ( response.params[3U], 0U );
	EXPECT_EQ( response.params[4U], 0U );
	EXPECT_EQ( response.params[5U], 0U );
	EXPECT_EQ( response.params[6U], 0U );
	EXPECT_EQ( response.params[7U], 0U );
	EXPECT_TRUE( success );
}
