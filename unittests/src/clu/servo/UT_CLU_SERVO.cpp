#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServo.h"
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
            EXPECT_CALL(mMockServos, GetServo(_)).WillOnce(Return(nullptr));
            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), 0xFFU);
            EXPECT_EQ(response.GetNbParams(), 3U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameAllAngle_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            const Frame  request(SERVO, GET_ALL);
            for (size_t servoId = 0U; servoId < NB_SERVOS; servoId++) {
                Component::Servo::Servo servo(mMockPca9685, servoId, 11U, 0U, 0U, 180U, false);
                EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
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
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, GET_ANGLE);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_ANGLE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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

        TEST_F(UT_CLU_SERVO, BuildFrameSetAngle_Npk) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_ANGLE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, GET_MIN);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_MIN);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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

        TEST_F(UT_CLU_SERVO, BuildFrameSetMin_Nok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_MIN);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::MockServo mMockservo;
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&mMockservo));
            EXPECT_CALL(mMockservo, SetMin(_)).WillOnce(Return(false));
            EXPECT_CALL(mMockservo, GetAngle()).WillOnce(Return(90U));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EClusterCommandGeneric::GENERIC);
            EXPECT_EQ(response.GetNbParams(), 3U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ((int8_t) response.Get1ByteParam(1U), (int8_t) Core::Status::CORE_ERROR_MIN);
            EXPECT_EQ(response.Get1ByteParam(2U), 90U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameGetMax_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, GET_MAX);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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

        TEST_F(UT_CLU_SERVO, BuildFrameSetMax_Nok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t angle = 111U;
            Frame             request(SERVO, SET_MAX);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::MockServo mMockservo;
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&mMockservo));
            EXPECT_CALL(mMockservo, SetMax(_)).WillOnce(Return(false));
            EXPECT_CALL(mMockservo, GetAngle()).WillOnce(Return(90U));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EClusterCommandGeneric::GENERIC);
            EXPECT_EQ(response.GetNbParams(), 3U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ((int8_t) response.Get1ByteParam(1U), (int8_t) Core::Status::CORE_ERROR_MAX);
            EXPECT_EQ(response.Get1ByteParam(2U), 90U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetMax_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t angle = 11;
            Frame             request(SERVO, SET_MAX);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, GET_OFFSET);
            request.Set1ByteParam(servoId);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 11U, 11U, 120U, false);
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
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t offset = 11;
            Frame             request(SERVO, SET_OFFSET);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(offset);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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

        TEST_F(UT_CLU_SERVO, BuildFrameSetOffset_Nok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            constexpr uint8_t angle = 111U;
            Frame             request(SERVO, SET_OFFSET);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(angle);
            Component::Servo::MockServo mMockservo;
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&mMockservo));
            EXPECT_CALL(mMockservo, SetOffset(_)).WillOnce(Return(false));
            EXPECT_CALL(mMockservo, GetOffset()).WillOnce(Return(90U));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EClusterCommandGeneric::GENERIC);
            EXPECT_EQ(response.GetNbParams(), 3U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ((int8_t) response.Get1ByteParam(1U), (int8_t) Core::Status::CORE_ERROR_MAX);
            EXPECT_EQ(response.Get1ByteParam(2U), 90U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameGetState_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, GET_STATE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(true);
            Component::Servo::MockServo mMockservo;
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&mMockservo));
            EXPECT_CALL(mMockservo, IsEnable()).WillOnce(Return(true));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_STATE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), true);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetState_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, SET_STATE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(true);
            Component::Servo::Servo servo(mMockPca9685, servoId, 90U, 0U, 11U, 120U, false);
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

        TEST_F(UT_CLU_SERVO, BuildFrameGetReverse_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, GET_REVERSE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(true);
            Component::Servo::MockServo mMockservo;
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&mMockservo));
            EXPECT_CALL(mMockservo, GetReverse()).WillOnce(Return(true));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_REVERSE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), true);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetReverse_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, SET_REVERSE);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(true);
            Component::Servo::MockServo mMockservo;
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&mMockservo));
            EXPECT_CALL(mMockservo, SetReverse(true)).Times(1U);

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::SET_REVERSE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), servoId);
            EXPECT_EQ(response.Get1ByteParam(1U), true);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameGetStatePca_Ok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, GET_STATE_PCA);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(true);
            EXPECT_CALL(mMockServos, GetState()).WillOnce(Return(true));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_STATE_PCA);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(response.Get1ByteParam(0U), true);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetStatePca_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request(SERVO, SET_STATE_PCA);
            request.Set1ByteParam(true);
            EXPECT_CALL(mMockServos, Enable()).Times(1U);

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_STATE_PCA);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(response.Get1ByteParam(0U), true);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrameSetStatePcaDisable_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request(SERVO, SET_STATE_PCA);
            request.Set1ByteParam(false);
            EXPECT_CALL(mMockServos, Disable()).Times(1U);

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), SERVO);
            EXPECT_EQ(response.GetCommandId(), EServoCommands::GET_STATE_PCA);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(response.Get1ByteParam(0U), false);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SERVO, BuildFrame_Nok) {
            Core::Status      success = Core::Status::CORE_ERROR;
            Frame             response;
            constexpr uint8_t servoId = 1U;
            Frame             request(SERVO, 15U);
            request.Set1ByteParam(servoId);
            request.Set1ByteParam(true);
            Component::Servo::MockServo mMockservo;
            EXPECT_CALL(mMockServos, GetServo(static_cast<EServos>(servoId)))
                    .WillOnce(Return(&mMockservo));

            success = mClusterServo.ExecuteFrame(request, response);

            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }
    } // namespace Servo
} // namespace Cluster
