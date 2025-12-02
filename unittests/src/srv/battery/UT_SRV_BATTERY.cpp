#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockBattery.h"
#include "../../../mock/srv/MockEventListener.h"

#include "../../../../src/Cluster/Battery/ClusterBattery.h"
#include "../../../../src/Service/Battery/ServiceBattery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Battery
    {
        class UT_SRV_BATTERY : public ::testing::Test {
        protected:
            UT_SRV_BATTERY() :
                             mMockEventListener()
                             , mMockBattery()
                             , mServiceBattery(mMockBattery, mMockEventListener) {
            }

            virtual void
            SetUp() {
                EXPECT_CALL(mMockBattery, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceBattery.Initialize());

                EXPECT_CALL(mMockBattery, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceBattery.Initialize());
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_SRV_BATTERY() = default;

            /* Mocks */
            StrictMock<Event::MockEventListener> mMockEventListener;
            StrictMock<Component::Battery::MockBattery> mMockBattery;

            /* Test class */
            ServiceBattery mServiceBattery;
        };

        TEST_F(UT_SRV_BATTERY, Update) {
            EXPECT_CALL(mMockBattery, Update(12340UL)).Times(1U);

            mServiceBattery.Update(12340UL);
        }

        TEST_F(UT_SRV_BATTERY, UpdatedBatteryState) {
            constexpr BatteryState batteryState = BatteryState::WARNING;
            constexpr uint8_t voltage = 10U;
            constexpr uint8_t intensity = 10U;
            Frame response;
            Cluster::Battery::ClusterBattery::BuildFrameState(batteryState, voltage, intensity, response);
            EXPECT_CALL(mMockEventListener, SendMessage(response)).Times(1U);

            mServiceBattery.Notified({batteryState});
        }
    }
}