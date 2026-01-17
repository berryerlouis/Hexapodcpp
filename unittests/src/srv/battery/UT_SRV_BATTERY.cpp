#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockBattery.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

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
            UT_SRV_BATTERY()
                : mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mMockBattery()
                , mServiceBattery(mMockBattery,
                                  mMockMessageInterface,
                                  mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockBattery, Initialize())
                        .WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR,
                          mServiceBattery.Initialize());

                EXPECT_CALL(mMockBattery, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceBattery.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_BATTERY() = default;

            /* Mocks */
            StrictMock<Event::MockEventDispatcherInterface>
                    mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>   mMockMessageInterface;
            StrictMock<Component::Battery::MockBattery> mMockBattery;

            /* Test class */
            ServiceBattery mServiceBattery;
        };

        TEST_F(UT_SRV_BATTERY, Update) {
            EXPECT_CALL(mMockBattery, Update(12340UL)).Times(1U);

            mServiceBattery.Update(12340UL);
        }

        TEST_F(UT_SRV_BATTERY, OnEvent) {
            const BatteryStruct battery = {.state = BatteryState::WARNING,
                                           .voltage = 10U,
                                           .intensity = 10U};
            const Event::Event  event = Event::Event(
                    BATTERY, EventType::EVENT_BATTERY_UPDATE, battery);
            mServiceBattery.OnEvent(event);
        }

        TEST_F(UT_SRV_BATTERY, UpdatedBatteryState) {
            const BatteryStruct battery = {.state = BatteryState::WARNING,
                                           .voltage = 10U,
                                           .intensity = 10U};
            Frame               response;
            const Event::Event  event = Event::Event(
                    BATTERY, EventType::EVENT_BATTERY_UPDATE, battery);
            Cluster::Battery::ClusterBattery::BuildFrameState(battery.state,
                                                              battery.voltage,
                                                              battery.intensity,
                                                              response);
            EXPECT_CALL(mMockMessageInterface, SendMessage(response)).Times(1U);
            EXPECT_CALL(mMockEventDispatcherInterface, DispatchEvent(event))
                    .Times(1U);

            mServiceBattery.Notified(battery);
        }
    } // namespace Battery
} // namespace Service
