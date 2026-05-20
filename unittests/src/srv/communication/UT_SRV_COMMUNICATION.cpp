#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/clu/MockClusters.h"
#include "../../../mock/cmp/MockCommunication.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Component/Button/ButtonState.h"
#include "../../../../src/Service/Communication/ServiceCommunication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Communication
    {
        class UT_SRV_COMMUNICATION : public ::testing::Test {
        protected:
            UT_SRV_COMMUNICATION()
                : mMockCommunication()
                , mMockClusters()
                , mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mServiceCommunication(mMockCommunication,
                                        mMockClusters,
                                        mMockMessageInterface,
                                        mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockCommunication, Initialize())
                        .WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceCommunication.Initialize());

                EXPECT_CALL(mMockCommunication, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceCommunication.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_COMMUNICATION() = default;


            /* Mocks */
            StrictMock<Component::Communication::MockCommunication> mMockCommunication;
            StrictMock<Cluster::Clusters::MockClusters>             mMockClusters;
            StrictMock<Event::MockEventDispatcherInterface>         mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>               mMockMessageInterface;

            /* Test class */
            ServiceCommunication mServiceCommunication;
        };

        TEST_F(UT_SRV_COMMUNICATION, Initialize_Update) {
            EXPECT_CALL(mMockCommunication, Update(12450UL)).Times(1U);
            EXPECT_CALL(mMockCommunication, GetMessage(_))
                    .WillOnce(Return(Core::Status::CORE_ERROR));
            mServiceCommunication.Update(12450UL);
        }

        TEST_F(UT_SRV_COMMUNICATION, Notified) {
            const CommunicationStruct state = NO_CLIENT;
            EXPECT_CALL(mMockEventDispatcherInterface, DispatchEvent(_)).Times(1U);
            mServiceCommunication.Notified({state});
        }

        TEST_F(UT_SRV_COMMUNICATION, OnEvent) {
            const Component::Button::ButtonStruct button(
                    {.state = Component::Button::ButtonState::PUSH, .delayMs = 10U});
            const Event::Event event = Event::Event(BUTTON, EventType::EVENT_BUTTON_UPDATE, button);
            mServiceCommunication.OnEvent(event);
        }
    } // namespace Communication
} // namespace Service
