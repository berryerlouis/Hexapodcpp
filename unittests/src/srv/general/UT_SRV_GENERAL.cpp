#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockLedPwm.h"
#include "../../../mock/cmp/MockSoftware.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Cluster/General/ClusterGeneral.h"
#include "../../../../src/Component/Button/ButtonState.h"
#include "../../../../src/Service/General/ServiceGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace General
    {
        class UT_SRV_GENERAL : public ::testing::Test {
        protected:
            UT_SRV_GENERAL()
                : mMockLedPwm()
                , mMockSoftware()
                , mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mServiceGeneral(mMockLedPwm,
                                  mMockSoftware,
                                  mMockMessageInterface,
                                  mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockLedPwm, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockSoftware, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceGeneral.Initialize());

                EXPECT_CALL(mMockLedPwm, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_CALL(mMockSoftware, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceGeneral.Initialize());

                EXPECT_CALL(mMockLedPwm, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockSoftware, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceGeneral.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_GENERAL() = default;

            /* Mocks */
            StrictMock<Component::LedPwm::MockLedPwm>       mMockLedPwm;
            StrictMock<Component::Software::MockSoftware>   mMockSoftware;
            StrictMock<Event::MockEventDispatcherInterface> mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>       mMockMessageInterface;

            /* Test class */
            ServiceGeneral mServiceGeneral;
        };

        TEST_F(UT_SRV_GENERAL, Update_Ok) {
            EXPECT_CALL(mMockLedPwm, Update(500U)).Times(1U);
            EXPECT_CALL(mMockSoftware, Update(500U)).Times(1U);
            mServiceGeneral.Update(500U);
        }

        TEST_F(UT_SRV_GENERAL, OnEventButton) {
            const Component::Button::ButtonStruct button(
                    {.state = Component::Button::ButtonState::PUSH, .delayMs = 10U});
            const Event::Event event = Event::Event(BUTTON, EventType::EVENT_BUTTON_UPDATE, button);

            EXPECT_CALL(mMockLedPwm, UpdateFrequency(5U)).Times(1U);

            mServiceGeneral.OnEvent(event);
        }

        TEST_F(UT_SRV_GENERAL, OnEventInit) {
            const Event::Event event =
                    Event::Event(BUTTON, EventType::EVENT_INIT_UPDATE, Core::Status::CORE_OK);

            mServiceGeneral.OnEvent(event);
        }

        TEST_F(UT_SRV_GENERAL, OnEventInitFail) {
            const Event::Event event =
                    Event::Event(BUTTON, EventType::EVENT_INIT_UPDATE, Core::Status::CORE_ERROR);

            mServiceGeneral.OnEvent(event);
        }
    } // namespace General
} // namespace Service
