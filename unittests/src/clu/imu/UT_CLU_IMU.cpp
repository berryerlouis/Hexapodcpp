#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockMpu9150.h"
#include "../../../../src/clu/ClusterImu.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;


TEST( ClusterImu, Execute_WrongCluster_Ko )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	Clusters::Frame          request;
	StrictMock <MockMpu9150> imu;
	ClusterImu clusterImu( imu );

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = clusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterImu, Execute_WrongCommand_Ko )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	Clusters::Frame          request;
	StrictMock <MockMpu9150> imu;
	ClusterImu clusterImu( imu );

	request.Build( Clusters::EClusters::IMU, 0x5FU );
	success = clusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterImu, Execute_ALL_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	Clusters::Frame          request;
	StrictMock <MockMpu9150> imu;
	ClusterImu clusterImu( imu );

	request.Build( Clusters::EClusters::IMU, Clusters::EImuCommands::ALL );
	EXPECT_CALL( imu, ReadAcc() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( imu, ReadGyr() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( imu, ReadMag() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( imu, ReadTemp() ).WillOnce( Return( 25 ) );

	success = clusterImu.Execute( request, response );
	
	EXPECT_EQ( response.clusterId, Clusters::EClusters::IMU );
	EXPECT_EQ( response.commandId, Clusters::EImuCommands::ALL );
	EXPECT_EQ( response.nbParams, 20U );
	EXPECT_TRUE( success );
}

TEST( ClusterImu, BuildFrameAcc_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	StrictMock <MockMpu9150> imu;
	ClusterImu clusterImu( imu );

	EXPECT_CALL( imu, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterImu.Initialize();

	EXPECT_CALL( imu, ReadAcc() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );

	clusterImu.BuildFrameAcc( response );
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

TEST( ClusterImu, BuildFrameGyr_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	StrictMock <MockMpu9150> imu;
	ClusterImu clusterImu( imu );

	EXPECT_CALL( imu, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterImu.Initialize();

	EXPECT_CALL( imu, ReadGyr() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );

	clusterImu.BuildFrameGyr( response );
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

TEST( ClusterImu, BuildFrameMag_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	StrictMock <MockMpu9150> imu;
	ClusterImu clusterImu( imu );

	EXPECT_CALL( imu, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterImu.Initialize();

	EXPECT_CALL( imu, ReadMag() ).WillOnce( Return( Mpu9150Interface::Vector3{ 5, 5, 5 } ) );

	clusterImu.BuildFrameMag( response );
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

TEST( ClusterImu, BuildFrameTemp_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	StrictMock <MockMpu9150> imu;
	ClusterImu clusterImu( imu );

	EXPECT_CALL( imu, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterImu.Initialize();

	EXPECT_CALL( imu, ReadTemp() ).WillOnce( Return( 25 ) );

	clusterImu.BuildFrameTmp( response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::IMU );
	EXPECT_EQ( response.commandId, Clusters::EImuCommands::TMP );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 25U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}
