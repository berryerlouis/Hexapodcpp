#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServos.h"

#include "../../../../src/Cluster/Servo/ClusterServo.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

namespace Cluster
{
    namespace Servo
    {
        class UT_CLU_SERVO : public ::testing::Test {
        protected:
            UT_CLU_SERVO()
                : mMockServos()
                , mMockPca9685()
                , mClusterServo(mMockServos) {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_CLU_SERVO() = default;

            /* Mocks */
            StrictMock<Component::Servos::MockServos>            mMockServos;
            StrictMock<Component::ServosController::MockPca9685> mMockPca9685;

            /* Test class */
            ClusterServo mClusterServo;
        };

        TEST_F(UT_CLU_SERVO, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(SERVO, 0x5FU);
            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), 0U);
            EXPECT_EQ(response.GetCommandId(), 0U);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameAllAngle_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            const Frame  request(SERVO, GET_ALL);
            for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++) {
                Component::Servo::Servo servo(
                        mMockPca9685, servoId, 11U, 0U, 0U, 180U, false);
                EXPECT_CALL(mMockServos,
                            GetServo(static_cast<EServos>(servoId)))
                        .WillOnce(Return(&servo));
            }
            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_ALL);
            EXPECT_EQ(response.GetNbParams(), 18U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameGetAngle_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            Frame             request(SERVO, GET_ANGLE);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_ANGLE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), 90U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetAngle_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_ANGLE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            servo.SetEnable(true);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .Times(2U)
                    .WillRepeatedly(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::SET_ANGLE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), 11U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }


        TEST_F(UT_CLU_SERVO, BuildFrameGetMin_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            Frame             request(SERVO, GET_MIN);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_MIN);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), 11U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetMin_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_MIN);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::SET_MIN);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), 11U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameGetMax_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            Frame             request(SERVO, GET_MAX);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_MAX);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), 120U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetMax_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_MAX);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::SET_MAX);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), 11U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameGetOffset_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            Frame             request(SERVO, GET_OFFSET);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 11U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_OFFSET);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), 11U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetOffset_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            constexpr uint8_t offset = 11;
            Frame             request(SERVO, SET_OFFSET);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(offset);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::SET_OFFSET);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), offset);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetState_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1;
            Frame             request(SERVO, SET_STATE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(true);
            Component::Servo::Servo servo(
                    mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&servo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::SET_STATE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), true);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    } // namespace Servo
} // namespace Cluster
