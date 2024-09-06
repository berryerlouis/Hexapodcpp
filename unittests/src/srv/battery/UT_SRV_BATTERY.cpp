#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockBattery.h"
#include "../../../mock/cor/MockEventMediatorInterface.h"

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
            UT_SRV_BATTERY() : mMockBattery(),
                               mMockEventMediatorInterface(),
                               mServiceBattery(mMockBattery) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockBattery, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_ERROR));
                EXPECT_FALSE(mServiceBattery.Initialize());

                EXPECT_CALL(mMockBattery, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_OK));
                EXPECT_CALL(mMockBattery, Attach( _ )).WillOnce(Return(Core::CoreStatus::CORE_OK));
                EXPECT_TRUE(mServiceBattery.Initialize());
                mServiceBattery.setMediator(&mMockEventMediatorInterface);
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_BATTERY() = default;

            /* Mocks */
            StrictMock<Component::Battery::MockBattery> mMockBattery;
            StrictMock<Core::MockEventMediatorInterface> mMockEventMediatorInterface;

            /* Test class */
            ServiceBattery mServiceBattery;
        };

        TEST_F(UT_SRV_BATTERY, Update) {
            EXPECT_CALL(mMockBattery, Update( 12340UL )).Times(1U);

            mServiceBattery.Update(12340UL);
        }

        TEST_F(UT_SRV_BATTERY, UpdatedBatteryState) {
            BatteryState batteryState = BatteryState::WARNING;
            Core::Event event;
            event.id = ClusterBase::EClusters::BATTERY;
            event.value = batteryState;
            EXPECT_CALL(mMockEventMediatorInterface, SendMessage( _ )).Times(1U);

            mServiceBattery.UpdatedBatteryState(batteryState);
        }
    }
}
