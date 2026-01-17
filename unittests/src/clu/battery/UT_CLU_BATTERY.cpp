#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/Cluster/Battery/ClusterBattery.h"
#include "../../../../src/Cluster/Constants.h"
#include "../../../mock/cmp/MockBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
    namespace Battery
    {
        class UT_CLU_BATTERY : public ::testing::Test {
        protected:
            UT_CLU_BATTERY()
                : mMockBattery()
                , mClusterBattery(mMockBattery) {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_CLU_BATTERY() = default;

            /* Mocks */
            StrictMock<Component::Battery::MockBattery> mMockBattery;

            /* Test class */
            ClusterBattery mClusterBattery;
        };


        TEST_F(UT_CLU_BATTERY, Execute_Command_Voltage_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            const Frame  request(BATTERY, GET_VOLTAGE);
            Frame        response;

            EXPECT_CALL(mMockBattery, GetVoltage()).WillOnce(Return(10U));

            success = mClusterBattery.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BATTERY);
            EXPECT_EQ(response.GetCommandId(), GET_VOLTAGE);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), 10U);
            EXPECT_EQ(response.Get1ByteParam(1U), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BATTERY, Execute_Command_Current_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            const Frame  request(BATTERY, GET_CURRENT);
            Frame        response;

            EXPECT_CALL(mMockBattery, GetIntensity()).WillOnce(Return(10U));

            success = mClusterBattery.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BATTERY);
            EXPECT_EQ(response.GetCommandId(), GET_CURRENT);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), 10U);
            EXPECT_EQ(response.Get1ByteParam(1U), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BATTERY, Execute_Command_status_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            const Frame  request(BATTERY, GET_BAT_STATUS);
            Frame        response;

            EXPECT_CALL(mMockBattery, GetVoltage()).WillOnce(Return(10U));
            EXPECT_CALL(mMockBattery, GetIntensity()).WillOnce(Return(10U));
            EXPECT_CALL(mMockBattery, GetState()).WillOnce(Return(NOMINAL));

            success = mClusterBattery.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), BATTERY);
            EXPECT_EQ(response.GetCommandId(), GET_BAT_STATUS);
            EXPECT_EQ(response.GetNbParams(), 5U);
            EXPECT_EQ(response.Get1ByteParam(0U), NOMINAL);
            EXPECT_EQ(response.Get1ByteParam(1U), 10U);
            EXPECT_EQ(response.Get1ByteParam(2U), 0U);
            EXPECT_EQ(response.Get1ByteParam(3U), 10U);
            EXPECT_EQ(response.Get1ByteParam(4U), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_BATTERY, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request;
            Frame        response;

            request.Build(BATTERY, 0x5FU);
            success = mClusterBattery.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), 0U);
            EXPECT_EQ(response.GetCommandId(), 0U);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }
    } // namespace Battery
} // namespace Cluster
