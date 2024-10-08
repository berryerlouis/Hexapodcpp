#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockBody.h"
#include "../../../../src/Cluster/Constants.h"
#include "../../../../src/Cluster/Body/ClusterBody.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
	namespace Body
	{
		class UT_CLU_BODY : public ::testing::Test {
		protected:
			UT_CLU_BODY() : mBodyMock(),
			                mClusterBody(mBodyMock) {
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
			Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			Frame request;
			Frame response;

			request.Build(BODY, 0x5FU);
			success = mClusterBody.ExecuteFrame(request, response);

			EXPECT_EQ(response.clusterId, 0U);
			EXPECT_EQ(response.commandId, 0U);
			EXPECT_EQ(response.nbParams, 0U);
			EXPECT_EQ(success, Core::CoreStatus::CORE_ERROR);
		}

		TEST_F(UT_CLU_BODY, Execute_SET_LEG_X_Y_Z_Ok) {
			Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
			Frame request;
			Frame response;
			uint8_t params[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

			EXPECT_CALL(mBodyMock, SetBodyPositionRotation( _, _, _ )).Times(1U);
			request.Build(BODY, EBodyCommands::SET_BODY_POS_ROT, params, 14U);
			success = mClusterBody.ExecuteFrame(request, response);

			EXPECT_EQ(response.clusterId, BODY);
			EXPECT_EQ(response.commandId, EBodyCommands::SET_BODY_POS_ROT);
			EXPECT_EQ(response.nbParams, 4U);
			EXPECT_EQ(success, Core::CoreStatus::CORE_OK);
		}

		TEST_F(UT_CLU_BODY, BuildFrameSetPosition_Ok) {
			Frame response;
			mClusterBody.BuildFrameSetPosition(response, 0);
			EXPECT_EQ(response.clusterId, BODY);
			EXPECT_EQ(response.commandId, EBodyCommands::SET_BODY_POS_ROT);
			EXPECT_EQ(response.nbParams, 4U);
		}
	}
}
