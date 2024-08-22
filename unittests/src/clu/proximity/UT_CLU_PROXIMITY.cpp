#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../../src/Cluster/Proximity/ClusterProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster {
namespace Proximity {
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
	StrictMock <Component::Proximity::MockSensorProximity> mMockSensorProximity;

	/* Test class */
	ClusterProximity mClusterProximity;
};

TEST_F( UT_CLU_PROXIMITY, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	Frame            request;

	request.Build( BODY, EBatteryCommands::GET_VOLTAGE );
	success = mClusterProximity.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_PROXIMITY, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	Frame            request;

	request.Build( PROXIMITY, 0x5FU );
	success = mClusterProximity.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_PROXIMITY, BuildFrameDistanceSrfLeft_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;

	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorsId::SRF_LEFT ) ).WillOnce( Return( 10U ) );

	success = mClusterProximity.BuildFrameDistance( EProximityCommands::US_LEFT, response );
	EXPECT_EQ( response.clusterId, PROXIMITY );
	EXPECT_EQ( response.commandId, EProximityCommands::US_LEFT );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_PROXIMITY, BuildFrameDistanceSrfRight_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorsId::SRF_RIGHT ) ).WillOnce( Return( 10U ) );

	success = mClusterProximity.BuildFrameDistance( EProximityCommands::US_RIGHT, response );
	EXPECT_EQ( response.clusterId, PROXIMITY );
	EXPECT_EQ( response.commandId, EProximityCommands::US_RIGHT );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_PROXIMITY, BuildFrameDistanceVlx_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;

	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorsId::VLX ) ).WillOnce( Return( 10U ) );
	success = mClusterProximity.BuildFrameDistance( EProximityCommands::LASER, response );
	EXPECT_EQ( response.clusterId, PROXIMITY );
	EXPECT_EQ( response.commandId, EProximityCommands::LASER );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}
}
}
