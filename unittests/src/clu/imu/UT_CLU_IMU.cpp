#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockMpu9150.h"
#include "../../../../src/Cluster/Imu/ClusterImu.h"
#include "../../../../src/Misc/Geometry/Geometry.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster {
namespace Imu {
class UT_CLU_IMU : public ::testing::Test {
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
	StrictMock <Component::Imu::MockMpu9150> mMockMpu9150;

	/* Test class */
	ClusterImu mClusterImu;
};

TEST_F( UT_CLU_IMU, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	Frame            request;

	request.Build( BODY, EBatteryCommands::GET_VOLTAGE );
	success = mClusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_IMU, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	Frame            request;

	request.Build( IMU, 0x5FU );
	success = mClusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_IMU, Execute_ALL_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	Frame            request;

	request.Build( IMU, EImuCommands::ALL );
	EXPECT_CALL( mMockMpu9150, ReadAcc() ).WillOnce( Return( Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( mMockMpu9150, ReadGyr() ).WillOnce( Return( Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( mMockMpu9150, ReadMag() ).WillOnce( Return( Vector3{ 5, 5, 5 } ) );
	EXPECT_CALL( mMockMpu9150, ReadTemp() ).WillOnce( Return( 25 ) );

	success = mClusterImu.Execute( request, response );

	EXPECT_EQ( response.clusterId, IMU );
	EXPECT_EQ( response.commandId, EImuCommands::ALL );
	EXPECT_EQ( response.nbParams, 20U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_IMU, BuildFrameAcc_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;

	EXPECT_CALL( mMockMpu9150, ReadAcc() ).WillOnce( Return( Vector3{ 5, 5, 5 } ) );

	success = mClusterImu.BuildFrameAcc( response );
	EXPECT_EQ( response.clusterId, IMU );
	EXPECT_EQ( response.commandId, EImuCommands::ACC );
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
	Frame            response;

	EXPECT_CALL( mMockMpu9150, ReadGyr() ).WillOnce( Return( Vector3{ 5, 5, 5 } ) );

	success = mClusterImu.BuildFrameGyr( response );
	EXPECT_EQ( response.clusterId, IMU );
	EXPECT_EQ( response.commandId, EImuCommands::GYR );
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
	Frame            response;

	EXPECT_CALL( mMockMpu9150, ReadMag() ).WillOnce( Return( Vector3{ 5, 5, 5 } ) );

	success = mClusterImu.BuildFrameMag( response );
	EXPECT_EQ( response.clusterId, IMU );
	EXPECT_EQ( response.commandId, EImuCommands::MAG );
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
	Frame            response;

	EXPECT_CALL( mMockMpu9150, ReadTemp() ).WillOnce( Return( 25 ) );

	success = mClusterImu.BuildFrameTmp( response );
	EXPECT_EQ( response.clusterId, IMU );
	EXPECT_EQ( response.commandId, EImuCommands::TMP );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 25U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}
}
}
