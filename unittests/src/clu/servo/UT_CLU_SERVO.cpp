#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTick.h"
#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServos.h"
#include "../../../mock/cmp/MockServo.h"

#include "../../../../src/clu/ClusterServo.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

using namespace Clusters;


TEST( ClusterServo, Execute_WrongCluster_Ko )
{
	Core::CoreStatus        success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame         response;
	Clusters::Frame         request;
	StrictMock <MockServos> servos;
	ClusterServo            clusterServo( servos );

	request.Build( Clusters::EClusters::BODY, Clusters::EBatteryCommands::GET_VOLTAGE );
	success = clusterServo.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterServo, Execute_WrongCommand_Ko )
{
	Core::CoreStatus        success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame         response;
	Clusters::Frame         request;
	StrictMock <MockServos> servos;
	ClusterServo            clusterServo( servos );

	request.Build( Clusters::EClusters::SERVO, 0x5FU );
	success = clusterServo.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST( ClusterServo, BuildFrameAngle_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	const uint8_t            servoId = 1U;
	StrictMock <MockServos>  servos;
	ClusterServo             clusterServo( servos );
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	EXPECT_CALL( servos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterServo.Initialize();

	Servo servo( pca9685, tick, servoId, 11U );
	EXPECT_CALL( servos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	clusterServo.BuildFrameAngle( servoId, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::SERVO );
	EXPECT_EQ( response.commandId, Clusters::EServoCommands::GET_ANGLE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 11U );
	EXPECT_TRUE( success );
}

TEST( ClusterServo, BuildFrameMinAngle_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	const uint8_t            servoId = 1U;
	StrictMock <MockServos>  servos;
	ClusterServo             clusterServo( servos );
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	EXPECT_CALL( servos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterServo.Initialize();

	Servo servo( pca9685, tick, servoId, 11U, 0U, 0U, 180U );
	EXPECT_CALL( servos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	clusterServo.BuildFrameMinAngle( servoId, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::SERVO );
	EXPECT_EQ( response.commandId, Clusters::EServoCommands::GET_MIN );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST( ClusterServo, BuildFrameMaxAngle_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	const uint8_t            servoId = 1U;
	StrictMock <MockServos>  servos;
	ClusterServo             clusterServo( servos );
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	EXPECT_CALL( servos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterServo.Initialize();

	Servo servo( pca9685, tick, servoId, 11U, 0U, 0U, 180U );
	EXPECT_CALL( servos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	clusterServo.BuildFrameMaxAngle( servoId, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::SERVO );
	EXPECT_EQ( response.commandId, Clusters::EServoCommands::GET_MAX );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 180U );
	EXPECT_TRUE( success );
}

TEST( ClusterServo, BuildFrameOffset_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	const uint8_t            servoId = 1U;
	StrictMock <MockServos>  servos;
	ClusterServo             clusterServo( servos );
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	EXPECT_CALL( servos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterServo.Initialize();

	Servo servo( pca9685, tick, servoId, 11U, 50U, 0U, 180U );
	EXPECT_CALL( servos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	clusterServo.BuildFrameOffset( servoId, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::SERVO );
	EXPECT_EQ( response.commandId, Clusters::EServoCommands::GET_OFFSET );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 50U );
	EXPECT_TRUE( success );
}

TEST( ClusterServo, BuildFrameState_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	const uint8_t            servoId = 1U;
	StrictMock <MockServos>  servos;
	ClusterServo             clusterServo( servos );
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	EXPECT_CALL( servos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterServo.Initialize();

	Servo servo( pca9685, tick, servoId, 11U, 50U, 0U, 180U );
	EXPECT_CALL( servos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	clusterServo.BuildFrameState( servoId, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::SERVO );
	EXPECT_EQ( response.commandId, Clusters::EServoCommands::GET_STATE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], true );
	EXPECT_TRUE( success );
}

TEST( ClusterServo, BuildFrameReverseFalse_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	const uint8_t            servoId = 1U;
	StrictMock <MockServos>  servos;
	ClusterServo             clusterServo( servos );
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	EXPECT_CALL( servos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterServo.Initialize();

	Servo servo( pca9685, tick, servoId, 11U, 50U, 0U, 180U );
	EXPECT_CALL( servos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	clusterServo.BuildFrameReverse( servoId, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::SERVO );
	EXPECT_EQ( response.commandId, Clusters::EServoCommands::GET_REVERSE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], false );
	EXPECT_TRUE( success );
}

TEST( ClusterServo, BuildFrameReverseTrue_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	Clusters::Frame          response;
	const uint8_t            servoId = 1U;
	StrictMock <MockServos>  servos;
	ClusterServo             clusterServo( servos );
	StrictMock <MockTick>    tick;
	StrictMock <MockPca9685> pca9685;

	EXPECT_CALL( servos, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = clusterServo.Initialize();

	Servo servo( pca9685, tick, servoId, 11U, 50U, 0U, 180U, true );
	EXPECT_CALL( servos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	clusterServo.BuildFrameReverse( servoId, response );

	EXPECT_EQ( response.clusterId, Clusters::EClusters::SERVO );
	EXPECT_EQ( response.commandId, Clusters::EServoCommands::GET_REVERSE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], true );
	EXPECT_TRUE( success );
}
