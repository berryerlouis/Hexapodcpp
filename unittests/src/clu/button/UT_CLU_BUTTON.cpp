#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockButton.h"
#include "../../../../src/Cluster/Constants.h"
#include "../../../../src/Cluster/Button/ClusterButton.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
    namespace Button
    {
        class UT_CLU_BUTTON : public ::testing::Test {
        protected:
            UT_CLU_BUTTON() :
                mMockButton(),
                mClusterButton(mMockButton) {
            }

            virtual void
            SetUp() {
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CLU_BUTTON() = default;

            /* Mocks */
            StrictMock<Component::Button::MockButton> mMockButton;

            /* Test class */
            ClusterButton mClusterButton;
        };

        TEST_F(UT_CLU_BUTTON, Execute_Command_status_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            const Frame request(BUTTON, GET_BP_STATUS);
            Frame response;

            EXPECT_CALL(mMockButton, Get()).WillOnce(Return(RELEASE));

            success = mClusterButton.ExecuteFrame(request, response);

            EXPECT_EQ(response.clusterId, BUTTON);
            EXPECT_EQ(response.commandId, GET_BP_STATUS);
            EXPECT_EQ(response.nbParams, 1U);
            EXPECT_EQ(response.params[0U], RELEASE);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BUTTON, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame request;
            Frame response;

            request.Build(BUTTON, 0x5FU);
            success = mClusterButton.ExecuteFrame(request, response);

            EXPECT_EQ(response.clusterId, 0U);
            EXPECT_EQ(response.commandId, 0U);
            EXPECT_EQ(response.nbParams, 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }
    }
}
