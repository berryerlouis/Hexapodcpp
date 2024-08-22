#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockBody.h"
#include "../../../../src/clu/ClusterBody.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

class UT_CLU_BODY : public ::testing::Test {
protected:
	UT_CLU_BODY() :
		mBodyMock(),
		mClusterBody( mBodyMock )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_BODY() = default;

	/* Mocks */
	StrictMock <MockBody> mBodyMock;

	/* Test class */
	ClusterBody mClusterBody;
};

TEST_F( UT_CLU_BODY, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  request;
	Clusters::Frame  response;

	request.Build( Clusters::EClusters::BATTERY, Clusters::EBodyCommands::SET_LEG_X_Y_Z );
	success = mClusterBody.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_BODY, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  request;
	Clusters::Frame  response;

	request.Build( Clusters::EClusters::BODY, 0x5FU );
	success = mClusterBody.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_BODY, Execute_SET_LEG_X_Y_Z_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  request;
	Clusters::Frame  response;
	uint8_t          params [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	EXPECT_CALL( mBodyMock, SetPositionRotation( _, _, _ ) ).Times( 1U );
	request.Build( Clusters::EClusters::BODY, Clusters::EBodyCommands::SET_LEG_X_Y_Z, params, 14U );
	success = mClusterBody.Execute( request, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::BODY );
	EXPECT_EQ( response.commandId, Clusters::EBodyCommands::SET_LEG_X_Y_Z );
	EXPECT_EQ( response.nbParams, 1U );
	EXPECT_EQ( response.params[0U], true );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_BODY, BuildFrameSetPosition_Ok )
{
	Clusters::Frame response;
	uint8_t         params [] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	mClusterBody.BuildFrameSetPosition( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::BODY );
	EXPECT_EQ( response.commandId, Clusters::EBodyCommands::SET_LEG_X_Y_Z );
	EXPECT_EQ( response.nbParams, 1U );
	EXPECT_EQ( response.params[0U], true );
}
