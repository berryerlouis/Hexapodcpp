#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSoftware.h"
#include "../../../../src/clu/ClusterGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

class UT_CLU_GENERAL : public ::testing::Test  {
protected:
	UT_CLU_GENERAL() :
		mMockSoftware(),
		mClusterGeneral( mMockSoftware )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_GENERAL() = default;

	/* Mocks */
	StrictMock <MockSoftware> mMockSoftware;

	/* Test class */
	ClusterGeneral mClusterGeneral;
};

TEST_F( UT_CLU_GENERAL, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  request;
	Clusters::Frame  response;

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = mClusterGeneral.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_GENERAL, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  request;
	Clusters::Frame  response;

	request.Build( Clusters::EClusters::GENERAL, 0x5FU );
	success = mClusterGeneral.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_GENERAL, Execute_VERSION_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  request;
	Clusters::Frame  response;

	EXPECT_CALL( mMockSoftware, GetVersion() ).WillOnce( Return( SoftwareInterface::Version{ 1, 0 } ) );
	request.Build( Clusters::EClusters::GENERAL, Clusters::EGeneralCommands::VERSION );
	success = mClusterGeneral.Execute( request, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::GENERAL );
	EXPECT_EQ( response.commandId, Clusters::EGeneralCommands::VERSION );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_GENERAL, BuildFrameGetVersion_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mClusterGeneral.Initialize();

	EXPECT_CALL( mMockSoftware, GetVersion() ).WillOnce( Return( SoftwareInterface::Version{ 1, 0 } ) );

	mClusterGeneral.BuildFrameGetVersion( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::GENERAL );
	EXPECT_EQ( response.commandId, Clusters::EGeneralCommands::VERSION );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 1U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_GENERAL, BuildFrameGetMinTime_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mClusterGeneral.Initialize();

	EXPECT_CALL( mMockSoftware, GetMinTime() ).WillOnce( Return( 50U ) );

	mClusterGeneral.BuildFrameGetMinTime( response );
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

TEST_F( UT_CLU_GENERAL, BuildFrameGetMaxTime_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	EXPECT_CALL( mMockSoftware, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mClusterGeneral.Initialize();

	EXPECT_CALL( mMockSoftware, GetMaxTime() ).WillOnce( Return( 50U ) );

	mClusterGeneral.BuildFrameGetMaxTime( response );
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
