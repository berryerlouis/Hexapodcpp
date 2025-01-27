#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockInputCapture.h"
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
                mMockInputCapture(),
                mButton(mMockInputCapture) {
            }

            virtual void
            SetUp() {
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CMP_BUTTON() = default;

            /* Mocks */
            StrictMock<Driver::InputCapture::MockInputCapture> mMockInputCapture;

            /* Test class */
            Button mButton;
        };

        TEST_F(UT_CMP_BUTTON, Initialize_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;

            EXPECT_CALL(mMockInputCapture, Initialize()).WillOnce(Return(Core::Status::CORE_OK));

            success = mButton.Initialize();

            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CMP_BUTTON, Get) {
            Core::Status success = Core::Status::CORE_ERROR;

            EXPECT_CALL(mMockInputCapture, Initialize()).WillOnce(Return(Core::Status::CORE_OK));

            success = mButton.Initialize();
            ButtonState status = mButton.Get();

            EXPECT_EQ(success, Core::Status::CORE_OK);
        }
    }
}
