#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/cmp/MockServos.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Component/Button/ButtonState.h"
#include "../../../../src/Service/Control/ServiceControl.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Control
    {
        class UT_SRV_CONTROL : public ::testing::Test {
        protected:
            UT_SRV_CONTROL()
                : mMockPca9685()
                , mMockServos()
                , mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mServiceControl(mMockServos,
                                  mMockMessageInterface,
                                  mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockServos, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceControl.Initialize());

                EXPECT_CALL(mMockServos, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceControl.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_CONTROL() = default;

            /* Mocks */
            StrictMock<Component::ServosController::MockPca9685> mMockPca9685;
            StrictMock<Component::Servos::MockServos>            mMockServos;
            StrictMock<Event::MockEventDispatcherInterface>      mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>            mMockMessageInterface;

            /* Test class */
            ServiceControl mServiceControl;
        };

        TEST_F(UT_SRV_CONTROL, Update_Ok) {
            EXPECT_CALL(mMockServos, Update(_)).Times(1U);
            EXPECT_CALL(mMockServos, GetServosController(_)).WillOnce(ReturnRef(mMockPca9685));
            EXPECT_CALL(mMockPca9685, Update(_)).Times(1U);

            mServiceControl.Update(0UL);
        }

        TEST_F(UT_SRV_CONTROL, Update_2Times_Ok) {
            EXPECT_CALL(mMockServos, Update(_)).Times(2U);
            EXPECT_CALL(mMockServos, GetServosController(0)).WillOnce(ReturnRef(mMockPca9685));
            EXPECT_CALL(mMockServos, GetServosController(1)).WillOnce(ReturnRef(mMockPca9685));
            EXPECT_CALL(mMockPca9685, Update(_)).Times(2U);

            mServiceControl.Update(0UL);
            mServiceControl.Update(0UL);
        }

        TEST_F(UT_SRV_CONTROL, OnEvent) {
            const Component::Button::ButtonStruct button(
                    {.state = Component::Button::ButtonState::PUSH, .delayMs = 10U});
            const Event::Event event = Event::Event(BUTTON, EventType::EVENT_BUTTON_UPDATE, button);
            mServiceControl.OnEvent(event);
        }
    } // namespace Control
} // namespace Service
