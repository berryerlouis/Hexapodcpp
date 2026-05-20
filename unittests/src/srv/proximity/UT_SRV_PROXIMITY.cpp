#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Cluster/Proximity/ClusterProximity.h"
#include "../../../../src/Component/Proximity/SensorProximity.h"
#include "../../../../src/Service/Proximity/ServiceProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Proximity
    {
        class UT_SRV_PROXIMITY : public ::testing::Test {
        protected:
            UT_SRV_PROXIMITY()
                : mMockSensorProximity()
                , mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mServiceProximity(mMockSensorProximity,
                                    mMockMessageInterface,
                                    mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockSensorProximity, Initialize())
                        .WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceProximity.Initialize());

                EXPECT_CALL(mMockSensorProximity, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceProximity.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_PROXIMITY() = default;

            /* Mocks */
            StrictMock<Component::Proximity::MockSensorProximity> mMockSensorProximity;
            StrictMock<Event::MockEventDispatcherInterface>       mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>             mMockMessageInterface;

            /* Test class */
            ServiceProximity mServiceProximity;
        };

        TEST_F(UT_SRV_PROXIMITY, Update) {
            EXPECT_CALL(mMockSensorProximity, Update(12450UL)).Times(1U);
            mServiceProximity.Update(12450UL);
        }

        TEST_F(UT_SRV_PROXIMITY, Detect) {
            constexpr SensorsId sensorId = SensorsId::SRF_LEFT;
            constexpr uint16_t  distance = 42U;

            Frame               response;
            Cluster::Proximity::ClusterProximity::BuildFrameDistanceUS(
                    sensorId, distance, response);
            EXPECT_CALL(mMockMessageInterface, SendMessage(response)).Times(1U);
            EXPECT_CALL(mMockEventDispatcherInterface, DispatchEvent(_)).Times(1U);
            mServiceProximity.Notified({sensorId, distance, nullptr});
        }

        TEST_F(UT_SRV_PROXIMITY, OnEvent) {
            const SensorsStruct sensor(
                    {.id = SensorsId::SRF_LEFT, .distance = 10U, .distanceArray = nullptr});
            const Event::Event event =
                    Event::Event(BATTERY, EventType::EVENT_SENSOR_UPDATE, sensor);
            mServiceProximity.OnEvent(event);
        }
    } // namespace Proximity
} // namespace Service
