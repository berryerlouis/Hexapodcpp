#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Cluster/General/ClusterGeneral.h"
#include "../../../mock/cmp/MockSoftware.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
    namespace General
    {
        class UT_CLU_GENERAL : public ::testing::Test {
        protected:
            UT_CLU_GENERAL()
                : mMockSoftware()
                , mClusterGeneral(mMockSoftware) {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_CLU_GENERAL() = default;

            /* Mocks */
            StrictMock<Component::Software::MockSoftware> mMockSoftware;

            /* Test class */
            ClusterGeneral mClusterGeneral;
        };

        TEST_F(UT_CLU_GENERAL, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            request.Build(GENERAL, 0x5FU);
            success = mClusterGeneral.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), 0U);
            EXPECT_EQ(response.GetCommandId(), 0U);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }

        TEST_F(UT_CLU_GENERAL, Execute_RESET_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            request.Build(GENERAL, EGeneralCommands::RESET);
            success = mClusterGeneral.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), GENERAL);
            EXPECT_EQ(response.GetCommandId(), EGeneralCommands::RESET);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_GENERAL, Execute_VERSION_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mMockSoftware, GetVersion())
                    .WillOnce(Return(SoftwareInterface::Version{1, 0}));
            request.Build(GENERAL, EGeneralCommands::VERSION);
            success = mClusterGeneral.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), GENERAL);
            EXPECT_EQ(response.GetCommandId(), EGeneralCommands::VERSION);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_GENERAL, Execute_MIN_EXECUTION_TIME_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mMockSoftware, GetMinTime()).WillOnce(Return(10UL));
            request.Build(GENERAL, EGeneralCommands::MIN_EXECUTION_TIME);
            success = mClusterGeneral.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), GENERAL);
            EXPECT_EQ(response.GetCommandId(), EGeneralCommands::MIN_EXECUTION_TIME);
            EXPECT_EQ(response.GetNbParams(), 9U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_GENERAL, Execute_MAX_EXECUTION_TIME_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            EXPECT_CALL(mMockSoftware, GetMaxTime()).WillOnce(Return(10UL));
            request.Build(GENERAL, EGeneralCommands::MAX_EXECUTION_TIME);
            success = mClusterGeneral.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), GENERAL);
            EXPECT_EQ(response.GetCommandId(), EGeneralCommands::MAX_EXECUTION_TIME);
            EXPECT_EQ(response.GetNbParams(), 9U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_GENERAL, Execute_RESET_EXECUTION_TIME_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            request.Build(GENERAL, EGeneralCommands::RESET_EXECUTION_TIME);
            success = mClusterGeneral.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), GENERAL);
            EXPECT_EQ(response.GetCommandId(), EGeneralCommands::RESET_EXECUTION_TIME);
            EXPECT_EQ(response.GetNbParams(), 1U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    } // namespace General
} // namespace Cluster
