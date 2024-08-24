#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTick.h"
#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServos.h"
#include "../../../mock/cmp/MockServo.h"

#include "../../../../src/Cluster/Servo/ClusterServo.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

namespace Cluster {
namespace Servo {
class UT_CLU_SERVO : public ::testing::Test {
protected:
	UT_CLU_SERVO() :
		mMockServos(),
		mMockTick(),
		mMockPca9685(),
		mClusterServo( mMockServos )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CLU_SERVO() = default;

	/* Mocks */
	StrictMock <Component::Servos::MockServos> mMockServos;
	StrictMock <Driver::Tick::MockTick> mMockTick;
	StrictMock <Component::ServosController::MockPca9685> mMockPca9685;

	/* Test class */
	ClusterServo mClusterServo;
};

TEST_F( UT_CLU_SERVO, Execute_WrongCluster_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	Frame            request;

	request.Build( BODY, EBatteryCommands::GET_VOLTAGE );
	success = mClusterServo.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_SERVO, Execute_WrongCommand_Ko )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	Frame            request;

	request.Build( SERVO, 0x5FU );
	success = mClusterServo.Execute( request, response );

	EXPECT_EQ( response.clusterId, 0U );
	EXPECT_EQ( response.commandId, 0U );
	EXPECT_EQ( response.nbParams, 0U );
	EXPECT_FALSE( success );
}

TEST_F( UT_CLU_SERVO, BuildFrameAngle_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	const uint8_t    servoId = 1U;

	Component::Servo::Servo servo( mMockPca9685, mMockTick, servoId, 11U );
	EXPECT_CALL( mMockServos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	success = mClusterServo.BuildFrameAngle( servoId, response );

	EXPECT_EQ( response.clusterId, SERVO );
	EXPECT_EQ( response.commandId, EServoCommands::GET_ANGLE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 11U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_SERVO, BuildFrameMinAngle_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	const uint8_t    servoId = 1U;

	Component::Servo::Servo servo( mMockPca9685, mMockTick, servoId, 11U, 0U, 0U, 180U );
	EXPECT_CALL( mMockServos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	success = mClusterServo.BuildFrameMinAngle( servoId, response );

	EXPECT_EQ( response.clusterId, SERVO );
	EXPECT_EQ( response.commandId, EServoCommands::GET_MIN );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 0U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_SERVO, BuildFrameMaxAngle_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	const uint8_t    servoId = 1U;

	Component::Servo::Servo servo( mMockPca9685, mMockTick, servoId, 11U, 0U, 0U, 180U );
	EXPECT_CALL( mMockServos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	success = mClusterServo.BuildFrameMaxAngle( servoId, response );

	EXPECT_EQ( response.clusterId, SERVO );
	EXPECT_EQ( response.commandId, EServoCommands::GET_MAX );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 180U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_SERVO, BuildFrameOffset_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	const uint8_t    servoId = 1U;

	Component::Servo::Servo servo( mMockPca9685, mMockTick, servoId, 11U, 50U, 0U, 180U );
	EXPECT_CALL( mMockServos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	success = mClusterServo.BuildFrameOffset( servoId, response );

	EXPECT_EQ( response.clusterId, SERVO );
	EXPECT_EQ( response.commandId, EServoCommands::GET_OFFSET );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], 50U );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_SERVO, BuildFrameState_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	const uint8_t    servoId = 1U;

	Component::Servo::Servo servo( mMockPca9685, mMockTick, servoId, 11U, 50U, 0U, 180U );
	EXPECT_CALL( mMockServos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	success = mClusterServo.BuildFrameState( servoId, response );

	EXPECT_EQ( response.clusterId, SERVO );
	EXPECT_EQ( response.commandId, EServoCommands::GET_STATE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], true );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_SERVO, BuildFrameReverseFalse_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	const uint8_t    servoId = 1U;

	Component::Servo::Servo servo( mMockPca9685, mMockTick, servoId, 11U, 50U, 0U, 180U );
	EXPECT_CALL( mMockServos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	success = mClusterServo.BuildFrameReverse( servoId, response );

	EXPECT_EQ( response.clusterId, SERVO );
	EXPECT_EQ( response.commandId, EServoCommands::GET_REVERSE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], false );
	EXPECT_TRUE( success );
}

TEST_F( UT_CLU_SERVO, BuildFrameReverseTrue_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	Frame            response;
	const uint8_t    servoId = 1U;

	Component::Servo::Servo servo( mMockPca9685, mMockTick, servoId, 11U, 50U, 0U, 180U, true );
	EXPECT_CALL( mMockServos, GetServo( servoId ) ).WillOnce( ReturnRef( servo ) );

	success = mClusterServo.BuildFrameReverse( servoId, response );

	EXPECT_EQ( response.clusterId, SERVO );
	EXPECT_EQ( response.commandId, EServoCommands::GET_REVERSE );
	EXPECT_EQ( response.nbParams, 2U );
	EXPECT_EQ( response.params[0U], servoId );
	EXPECT_EQ( response.params[1U], true );
	EXPECT_TRUE( success );
}
}
}
