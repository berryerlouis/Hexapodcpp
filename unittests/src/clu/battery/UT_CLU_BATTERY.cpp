#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockBattery.h"
#include "../../../../src/Cluster/Constants.h"
#include "../../../../src/Cluster/Battery/ClusterBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster {
namespace Battery {
class UT_CLU_BATTERY : public ::testing::Test {
protected:
	UT_CLU_BATTERY() :
		mMockBattery(),
		mClusterBattery( mMockBattery )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_BATTERY() = default;

	/* Mocks */
	StrictMock <Component::Battery::MockBattery> mMockBattery;

	/* Test class */
	ClusterBattery mClusterBattery;
};


TEST_F( UT_CLU_BATTERY, Execute_WrongCluster_Ko1 )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            request;
	Frame            response;

	request.Build( BODY, EBatteryCommands::GET_VOLTAGE );
	success = mClusterBattery.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_BATTERY, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            request;
	Frame            response;

	request.Build( BATTERY, 0x5FU );
	success = mClusterBattery.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_BATTERY, Execute_GET_VOLTAGE_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            request;
	Frame            response;

	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	request.Build( BATTERY, EBatteryCommands::GET_VOLTAGE );
	success = mClusterBattery.Execute( request, response );

	EXPECT_EQ( response.clusterId, BATTERY );
	EXPECT_EQ( response.commandId, EBatteryCommands::GET_VOLTAGE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], 10U );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_BATTERY, BuildFrameState_Nominal_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;

	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockBattery, GetState() ).WillOnce( Return( BatteryState::NOMINAL ) );

	success = mClusterBattery.BuildFrameState( response );
	EXPECT_EQ( response.clusterId, BATTERY );
	EXPECT_EQ( response.commandId, EBatteryCommands::GET_BAT_STATUS );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], BatteryState::NOMINAL );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_BATTERY, BuildFrameState_Critical_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;

	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockBattery, GetState() ).WillOnce( Return( BatteryState::CRITICAL ) );

	success = mClusterBattery.BuildFrameState( response );
	EXPECT_EQ( response.clusterId, BATTERY );
	EXPECT_EQ( response.commandId, EBatteryCommands::GET_BAT_STATUS );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], BatteryState::CRITICAL );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_BATTERY, BuildFrameState_Warning_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;

	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockBattery, GetState() ).WillOnce( Return( BatteryState::WARNING ) );

	success = mClusterBattery.BuildFrameState( response );
	EXPECT_EQ( response.clusterId, BATTERY );
	EXPECT_EQ( response.commandId, EBatteryCommands::GET_BAT_STATUS );
	EXPECT_EQ( response.nbParams, 3U );
	EXPECT_EQ( response.params[0U], BatteryState::WARNING );
	EXPECT_EQ( response.params[1U], 10U );
	EXPECT_EQ( response.params[2U], 0U );
	EXPECT_TRUE( success );
}
}
}
