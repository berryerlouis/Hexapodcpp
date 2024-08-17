#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockMpu9150.h"
#include "../../../../src/clu/ClusterImu.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

class UT_CLU_IMU : public ::testing::Test  {
protected:
	UT_CLU_IMU() :
		mMockMpu9150(),
		mClusterImu( mMockMpu9150 )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_IMU() = default;

	/* Mocks */
	StrictMock <MockMpu9150> mMockMpu9150;

	/* Test class */
	ClusterImu mClusterImu;
};

TEST_F( UT_CLU_IMU, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	Clusters::Frame  request;

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = mClusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_IMU, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	Clusters::Frame  request;

	request.Build( Clusters::EClusters::IMU, 0x5FU );
	success = mClusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_IMU, Execute_ALL_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	Clusters::Frame  request;

	request.Build( Clusters::EClusters::IMU, Clusters::EImuCommands::ALL );
	EXPECT_CALL( mMockMpu9150, ReadAcc() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( mMockMpu9150, ReadGyr() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( mMockMpu9150, ReadMag() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( mMockMpu9150, ReadTemp() ).WillOnce( Return( 25 ) );

	success = mClusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::IMU );
	EXPECT_EQ( response.commandId, Clusters::EImuCommands::ALL );
	EXPECT_EQ( response.nbParams, 20U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_IMU, BuildFrameAcc_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	EXPECT_CALL( mMockMpu9150, ReadAcc() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );

	success = mClusterImu.BuildFrameAcc( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::IMU );
	EXPECT_EQ( response.commandId, Clusters::EImuCommands::ACC );
	EXPECT_EQ( response.nbParams, 6U );
	EXPECT_EQ( response.params[0U], 5U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.params[2U], 5U );
	EXPECT_EQ( response.params[3U], 0U );
	EXPECT_EQ( response.params[4U], 5U );
	EXPECT_EQ( response.params[5U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_IMU, BuildFrameGyr_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	
	EXPECT_CALL( mMockMpu9150, ReadGyr() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );

	success = mClusterImu.BuildFrameGyr( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::IMU );
	EXPECT_EQ( response.commandId, Clusters::EImuCommands::GYR );
	EXPECT_EQ( response.nbParams, 6U );
	EXPECT_EQ( response.params[0U], 5U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.params[2U], 5U );
	EXPECT_EQ( response.params[3U], 0U );
	EXPECT_EQ( response.params[4U], 5U );
	EXPECT_EQ( response.params[5U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_IMU, BuildFrameMag_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	EXPECT_CALL( mMockMpu9150, ReadMag() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );

	success = mClusterImu.BuildFrameMag( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::IMU );
	EXPECT_EQ( response.commandId, Clusters::EImuCommands::MAG );
	EXPECT_EQ( response.nbParams, 6U );
	EXPECT_EQ( response.params[0U], 5U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_EQ( response.params[2U], 5U );
	EXPECT_EQ( response.params[3U], 0U );
	EXPECT_EQ( response.params[4U], 5U );
	EXPECT_EQ( response.params[5U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_IMU, BuildFrameTemp_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

	EXPECT_CALL( mMockMpu9150, ReadTemp() ).WillOnce( Return( 25 ) );

	success = mClusterImu.BuildFrameTmp( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::IMU );
	EXPECT_EQ( response.commandId, Clusters::EImuCommands::TMP );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 25U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}
