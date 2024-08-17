#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../../src/clu/ClusterProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Clusters;

class UT_CLU_PROXIMITY : public ::testing::Test {
protected:
	UT_CLU_PROXIMITY() :
		mMockSensorProximity(),
		mClusterProximity( mMockSensorProximity )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_PROXIMITY() = default;

	/* Mocks */
	StrictMock <MockSensorProximity> mMockSensorProximity;

	/* Test class */
	ClusterProximity mClusterProximity;
};

TEST_F( UT_CLU_PROXIMITY, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	Clusters::Frame  request;

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = mClusterProximity.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_PROXIMITY, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;
	Clusters::Frame  request;

	request.Build( Clusters::EClusters::PROXIMITY, 0x5FU );
	success = mClusterProximity.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_PROXIMITY, BuildFrameDistanceSrfLeft_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

<<<<<<< HEAD
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorsId::SRF_LEFT ) ).WillOnce( Return( 10U ) );
=======
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( 10U ) );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)

	success = mClusterProximity.BuildFrameDistance( EProximityCommands::US_LEFT, response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::PROXIMITY );
	EXPECT_EQ( response.commandId, Clusters::EProximityCommands::US_LEFT );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_PROXIMITY, BuildFrameDistanceSrfRight_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

<<<<<<< HEAD
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorsId::SRF_RIGHT ) ).WillOnce( Return( 10U ) );
=======
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( 10U ) );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)

	success = mClusterProximity.BuildFrameDistance( EProximityCommands::US_RIGHT, response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::PROXIMITY );
	EXPECT_EQ( response.commandId, Clusters::EProximityCommands::US_RIGHT );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_PROXIMITY, BuildFrameDistanceVlx_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame  response;

<<<<<<< HEAD
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorsId::VLX ) ).WillOnce( Return( 10U ) );
=======
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( 10U ) );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)

	success = mClusterProximity.BuildFrameDistance( EProximityCommands::LASER, response );
	EXPECT_EQ( response.clusterId, Clusters::EClusters::PROXIMITY );
	EXPECT_EQ( response.commandId, Clusters::EProximityCommands::LASER );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}
