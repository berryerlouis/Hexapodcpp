#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockGpio.h"
#include "../../../mock/drv/MockTick.h"
#include "../../../../src/Component/Button/Button.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
    namespace Button
    {
        class UT_CMP_BUTTON : public ::testing::Test {
        protected:
            UT_CMP_BUTTON() :
                mMockGpio(),
                mMockTick(),
                mButton(mMockGpio, mMockTick) {
            }

            virtual void
            SetUp() {
                Core::Status success = Core::Status::CORE_ERROR;
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                success = mButton.Initialize();
                EXPECT_EQ(success, Core::Status::CORE_OK);
                const ButtonState status = mButton.Get();
                EXPECT_EQ(status, RELEASE);
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CMP_BUTTON() = default;

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio> mMockGpio;
            StrictMock<Driver::Tick::MockTick> mMockTick;

            /* Test class */
            Button mButton;
        };
    }
}
