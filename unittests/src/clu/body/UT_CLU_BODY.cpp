#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/Cluster/Body/ClusterBody.h"
#include "../../../../src/Cluster/Constants.h"
#include "../../../mock/bot/MockBody.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
    namespace Body
    {
        class UT_CLU_BODY : public ::testing::Test {
        protected:
            UT_CLU_BODY()
                : mBodyMock()
                , mClusterBody(mBodyMock) {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_CLU_BODY() = default;

            /* Mocks */
            StrictMock<Bot::Body::MockBody> mBodyMock;

            /* Test class */
            ClusterBody mClusterBody;
        };

        TEST_F(UT_CLU_BODY, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            request.Build(BODY, 0x5FU);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), 0U);
            EXPECT_EQ(response.GetCommandId(), 0U);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_LEG_X_Y_Z_Ok) {
            Core::Status  success = Core::Status::CORE_ERROR;
            Frame         request;
            Frame         response;
            const uint8_t params[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            EXPECT_CALL(mBodyMock, SetBodyPositionRotation(_, _, _)).Times(1U);
            request.Build(BODY, EBodyCommands::SET_BODY_POS_ROT, params, 14U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::SET_BODY_POS_ROT);
            EXPECT_EQ(response.GetNbParams(), 4U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, BuildFrameSetPosition_Ok) {
            Frame response;
            mClusterBody.BuildFrameSetBodyPosition(response, 0);
            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::SET_BODY_POS_ROT);
            EXPECT_EQ(response.GetNbParams(), 4U);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_LEG_POS_ROT_Ok) {
            Core::Status  success = Core::Status::CORE_ERROR;
            Frame         request;
            Frame         response;
            const uint8_t params[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            EXPECT_CALL(mBodyMock, SetLegPositionRotation(_, _, _)).Times(1U);
            request.Build(BODY, EBodyCommands::SET_LEG_POS_ROT, params, 9U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::SET_LEG_POS_ROT);
            EXPECT_EQ(response.GetNbParams(), 4U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_WALK_STATUS_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, UpdateWalkStatus(Move::Walk::EWalkStatus::WALKING, 10U))
                    .Times(1U);
            EXPECT_CALL(mBodyMock, GetWalkStatus()).Times(1U);
            request.Build(BODY, EBodyCommands::SET_WALK_STATUS);
            request.Set1ByteParam((uint8_t) Move::Walk::EWalkStatus::WALKING);
            request.Set2BytesParam(10U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::SET_WALK_STATUS);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_GET_DIRECTION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, GetDirection()).Times(1U);
            request.Build(BODY, EBodyCommands::GET_DIRECTION);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_DIRECTION);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_DIRECTION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, SetDirection(_)).Times(1U);
            EXPECT_CALL(mBodyMock, GetDirection()).Times(1U);
            request.Build(BODY, EBodyCommands::SET_DIRECTION);
            request.Set2BytesParam(10U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_DIRECTION);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_GET_ROTATION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, GetRotation()).Times(1U);
            EXPECT_CALL(mBodyMock, GetRotationClockWize()).Times(1U);
            request.Build(BODY, EBodyCommands::GET_ROTATION);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_ROTATION);
            EXPECT_EQ(response.GetNbParams(), 3U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_ROTATION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, SetRotation(_, true)).Times(1U);
            EXPECT_CALL(mBodyMock, GetRotation()).Times(1U);
            EXPECT_CALL(mBodyMock, GetRotationClockWize()).Times(1U);
            request.Build(BODY, EBodyCommands::SET_ROTATION);
            request.Set2BytesParam(10U);
            request.Set1ByteParam(true);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_ROTATION);
            EXPECT_EQ(response.GetNbParams(), 3U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }


        TEST_F(UT_CLU_BODY, Execute_GET_AMPLITUDE_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, GetAmplitude()).Times(1U);
            request.Build(BODY, EBodyCommands::GET_AMPLITUDE);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_AMPLITUDE);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_AMPLITUDE_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, SetAmplitude(_)).Times(1U);
            EXPECT_CALL(mBodyMock, GetAmplitude()).Times(1U);
            request.Build(BODY, EBodyCommands::SET_AMPLITUDE);
            request.Set2BytesParam(10U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_AMPLITUDE);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
        TEST_F(UT_CLU_BODY, Execute_GET_ELEVATION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, GetElevation()).Times(1U);
            request.Build(BODY, EBodyCommands::GET_ELEVATION);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_ELEVATION);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_ELEVATION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, SetElevation(_)).Times(1U);
            EXPECT_CALL(mBodyMock, GetElevation()).Times(1U);
            request.Build(BODY, EBodyCommands::SET_ELEVATION);
            request.Set2BytesParam(10U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_ELEVATION);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_GET_DURATION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, GetDuration()).Times(1U);
            request.Build(BODY, EBodyCommands::GET_DURATION);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_DURATION);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_DURATION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, SetDuration(_)).Times(1U);
            EXPECT_CALL(mBodyMock, GetDuration()).Times(1U);
            request.Build(BODY, EBodyCommands::SET_DURATION);
            request.Set2BytesParam(10U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_DURATION);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }


        TEST_F(UT_CLU_BODY, Execute_GET_GAIT_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, GetGait()).Times(1U);
            request.Build(BODY, EBodyCommands::GET_GAIT);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_GAIT);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BODY, Execute_SET_GAIT_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mBodyMock, SetGait(_)).Times(1U);
            EXPECT_CALL(mBodyMock, GetGait()).Times(1U);
            request.Build(BODY, EBodyCommands::SET_GAIT);
            request.Set2BytesParam(10U);
            success = mClusterBody.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BODY);
            EXPECT_EQ(response.GetCommandId(), EBodyCommands::GET_GAIT);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    } // namespace Body
} // namespace Cluster
