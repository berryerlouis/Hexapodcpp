#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../../src/clu/ClusterProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

TEST( ClusterProximity, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	Clusters::Frame  request;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = clusterProximity.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterProximity, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	Clusters::Frame  request;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );

	request.Build( Clusters::EClusters::PROXIMITY, 0x5FU );
	success = clusterProximity.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterProximity, BuildFrameDistanceSrfLeft_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );

	EXPECT_CALL( sensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterProximity.Initialize();

	EXPECT_CALL( sensorProximity, GetDistance( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( 10U ) );

	clusterProximity.BuildFrameDistance( EProximityCommands::US_LEFT, response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::PROXIMITY );
	EXPECT_EQ( response.commandId, Clusters::EProximityCommands::US_LEFT );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 0U );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_TRUE( success );
}

TEST( ClusterProximity, BuildFrameDistanceSrfRight_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );

	EXPECT_CALL( sensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterProximity.Initialize();

	EXPECT_CALL( sensorProximity, GetDistance( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( 10U ) );

	clusterProximity.BuildFrameDistance( EProximityCommands::US_RIGHT, response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::PROXIMITY );
	EXPECT_EQ( response.commandId, Clusters::EProximityCommands::US_RIGHT );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 0U );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_TRUE( success );
}

TEST( ClusterProximity, BuildFrameDistanceVlx_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );

	EXPECT_CALL( sensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterProximity.Initialize();

	EXPECT_CALL( sensorProximity, GetDistance( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( 10U ) );

	clusterProximity.BuildFrameDistance( EProximityCommands::LASER, response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::PROXIMITY );
	EXPECT_EQ( response.commandId, Clusters::EProximityCommands::LASER );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 0U );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_TRUE( success );
}
