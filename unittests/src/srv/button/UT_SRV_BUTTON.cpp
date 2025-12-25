#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockButton.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Cluster/Button/ClusterButton.h"
#include "../../../../src/Service/Button/ServiceButton.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Button
    {
        class UT_SRV_BUTTON : public ::testing::Test {
        protected:
            UT_SRV_BUTTON() :
                mMockEventDispatcherInterface(),
                mMockMessageInterface(),
                mMockButton(),
                mServiceButton(mMockButton,
                               mMockMessageInterface,
                               mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockButton, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceButton.Initialize());

                EXPECT_CALL(mMockButton, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceButton.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_BUTTON() = default;

            /* Mocks */
            StrictMock<Event::MockEventDispatcherInterface> mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>       mMockMessageInterface;
            StrictMock<Component::Button::MockButton>       mMockButton;

            /* Test class */
            ServiceButton mServiceButton;
        };

        TEST_F(UT_SRV_BUTTON,
               Update) {
            EXPECT_CALL(mMockButton, Update(12340UL)).Times(1U);
            mServiceButton.Update(12340UL);
        }

        TEST_F(UT_SRV_BUTTON,
               UpdatedButtonState) {
            constexpr ButtonState ButtonState = ButtonState::RELEASE;

            Frame                 response;
            Cluster::Button::ClusterButton::BuildFrameGetButtonState(ButtonState, response);
            EXPECT_CALL(mMockMessageInterface, SendMessage(response)).Times(1U);
            EXPECT_CALL(mMockEventDispatcherInterface, DispatchEvent(_)).Times(1U);
            mServiceButton.Notified({ButtonState, 10U});
        }
    } // namespace Button
} // namespace Service
