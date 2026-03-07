#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Cluster/Proximity/ClusterProximity.h"
#include "../../../mock/cmp/MockSensorProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
    namespace Proximity
    {
        class UT_CLU_PROXIMITY : public ::testing::Test {
        protected:
            UT_CLU_PROXIMITY()
                : mMockSensorProximity()
                , mClusterProximity(mMockSensorProximity) {
            }

            virtual void SetUp() {
            }

            virtual void TearDown() {
            }

            virtual ~UT_CLU_PROXIMITY() = default;

            /* Mocks */
            StrictMock<Component::Proximity::MockSensorProximity> mMockSensorProximity;

            /* Test class */
            ClusterProximity mClusterProximity;
        };


        TEST_F(UT_CLU_PROXIMITY, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        response;
            Frame        request;

            request.Build(PROXIMITY, 0x5FU);
            success = mClusterProximity.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), 0U);
            EXPECT_EQ(response.GetCommandId(), 0U);
            EXPECT_EQ(response.GetNbParams(), 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }

        TEST_F(UT_CLU_PROXIMITY, Execute_Command_UsLeft_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            const Frame  request(PROXIMITY, US_LEFT);
            Frame        response;
            EXPECT_CALL(mMockSensorProximity, GetDistance(static_cast<SensorsId>(US_LEFT)))
                    .WillOnce(Return(10U));
            success = mClusterProximity.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), PROXIMITY);
            EXPECT_EQ(response.GetCommandId(), US_LEFT);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), 10U);
            EXPECT_EQ(response.Get1ByteParam(1U), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_PROXIMITY, Execute_Command_UsRight_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            const Frame  request(PROXIMITY, US_RIGHT);
            Frame        response;
            EXPECT_CALL(mMockSensorProximity, GetDistance(static_cast<SensorsId>(US_RIGHT)))
                    .WillOnce(Return(10U));
            success = mClusterProximity.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), PROXIMITY);
            EXPECT_EQ(response.GetCommandId(), US_RIGHT);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), 10U);
            EXPECT_EQ(response.Get1ByteParam(1U), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_PROXIMITY, Execute_Command_Laser_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            const Frame  request(PROXIMITY, LASER);
            Frame        response;
            EXPECT_CALL(mMockSensorProximity, GetDistance(static_cast<SensorsId>(LASER)))
                    .WillOnce(Return(10U));
            success = mClusterProximity.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), PROXIMITY);
            EXPECT_EQ(response.GetCommandId(), LASER);
            EXPECT_EQ(response.GetNbParams(), 2U);
            EXPECT_EQ(response.Get1ByteParam(0U), 10U);
            EXPECT_EQ(response.Get1ByteParam(1U), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_PROXIMITY, Execute_Command_SetThreshold_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame        request(PROXIMITY, SET_THRESHOLD);
            request.Set1ByteParam(LASER);
            request.Set2BytesParam(10U);

            Frame response;
            EXPECT_CALL(mMockSensorProximity, SetThreshold(static_cast<SensorsId>(LASER), 10U))
                    .WillOnce(Return(Core::Status::CORE_OK));
            success = mClusterProximity.ExecuteFrame(request, response);

            EXPECT_EQ(response.GetClusterId(), PROXIMITY);
            EXPECT_EQ(response.GetCommandId(), SET_THRESHOLD);
            EXPECT_EQ(response.GetNbParams(), 3U);
            EXPECT_EQ(response.Get1ByteParam(0U), 2U);
            EXPECT_EQ(response.Get1ByteParam(1U), 10U);
            EXPECT_EQ(response.Get1ByteParam(2U), 0U);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    } // namespace Proximity
} // namespace Cluster
