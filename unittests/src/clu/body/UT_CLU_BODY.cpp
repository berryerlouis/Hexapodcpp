#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockBody.h"
#include "../../../../src/clu/ClusterBody.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

TEST( ClusterBody, Initialize_Ok )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame       response;
	StrictMock <MockBody> body;
	ClusterBody           clusterBody( body );

	EXPECT_CALL( body, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterBody.Initialize();

	EXPECT_TRUE( success );
}

TEST( ClusterBody, Execute_WrongCluster_Ko )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame       request;
	Clusters::Frame       response;
	StrictMock <MockBody> body;
	ClusterBody           clusterBody( body );

	request.Build( Clusters::EClusters::BATTERY, Clusters::EBodyCommands::SET_LEG_X_Y_Z );
	success = clusterBody.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterBody, Execute_WrongCommand_Ko )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame       request;
	Clusters::Frame       response;
	StrictMock <MockBody> body;
	ClusterBody           clusterBody( body );

	request.Build( Clusters::EClusters::BODY, 0x5FU );
	success = clusterBody.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterBody, Execute_SET_LEG_X_Y_Z_Ok )
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame       request;
	Clusters::Frame       response;
	StrictMock <MockBody> body;
	ClusterBody           clusterBody( body );
	uint8_t params [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	EXPECT_CALL( body, SetPositionRotation( _, _, _ ) ).Times( 1U );
	request.Build( Clusters::EClusters::BODY, Clusters::EBodyCommands::SET_LEG_X_Y_Z, params, 14U );
	success = clusterBody.Execute( request, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BODY );
	EXPECT_EQ( response.commandId, Clusters::EBodyCommands::SET_LEG_X_Y_Z );
	EXPECT_EQ( response.nbParams, 1U );
	EXPECT_EQ( response.params[0U], true );
	EXPECT_TRUE( success );
}

TEST( ClusterBody, BuildFrameSetPosition_Ok )
{
	Clusters::Frame       response;
	StrictMock <MockBody> body;
	ClusterBody           clusterBody( body );
	uint8_t params [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	clusterBody.BuildFrameSetPosition( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::BODY );
	EXPECT_EQ( response.commandId, Clusters::EBodyCommands::SET_LEG_X_Y_Z );
	EXPECT_EQ( response.nbParams, 1U );
	EXPECT_EQ( response.params[0U], true );
}
