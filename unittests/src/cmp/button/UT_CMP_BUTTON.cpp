#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/Component/Button/Button.h"
#include "../../../mock/drv/MockGpio.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
    namespace Button
    {
        class UT_CMP_BUTTON : public ::testing::Test {
        protected:
            UT_CMP_BUTTON()
                : mMockGpio()
                , mButton(mMockGpio) {
            }

            virtual void SetUp() {
                Core::Status success = Core::Status::CORE_ERROR;
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                success = mButton.Initialize();
                EXPECT_EQ(success, Core::Status::CORE_OK);
                const ButtonState status = mButton.Get();
                EXPECT_EQ(status, RELEASE);
            }

            virtual void TearDown() {
            }

            virtual ~UT_CMP_BUTTON() = default;

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio> mMockGpio;

            /* Test class */
            Button mButton;
        };

        TEST_F(UT_CMP_BUTTON, Hit) {
            EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(true));
            mButton.Hit();
            EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(false));
            mButton.Hit();
        }

        TEST_F(UT_CMP_BUTTON, Update) {
            mButton.Update(0UL);
        }

        TEST_F(UT_CMP_BUTTON, Get) {
            const ButtonState state = mButton.Get();
            EXPECT_EQ(RELEASE, state);
        }
    } // namespace Button
} // namespace Component
