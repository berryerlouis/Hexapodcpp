#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockGpio.h"
#include "../../../../src/Component/Led/Led.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

namespace Component
{
    namespace Led
    {
        class UT_CMP_LED : public ::testing::Test {
        protected:
            UT_CMP_LED() :
                mMockGpio(),
                mLed(mMockGpio) {
            }

            virtual void
            SetUp() {
                EXPECT_CALL(mMockGpio, Reset()).WillOnce(Return(Core::Status::CORE_OK));
                Driver::Gpio::SGpio gpio;
                EXPECT_CALL(mMockGpio, GetPin()).WillOnce(ReturnRef(gpio));
                const Core::Status success = mLed.Initialize();
                EXPECT_EQ(success, Core::Status::CORE_OK);
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CMP_LED() = default;

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio> mMockGpio;

            /* Test class */
            Led mLed;
        };


        TEST_F(UT_CMP_LED, On) {
            Core::Status success = Core::Status::CORE_ERROR;

            EXPECT_CALL(mMockGpio, Set()).WillOnce(Return(Core::Status::CORE_OK));

            success = mLed.On();

            const Led::LedState status = mLed.Get();

            EXPECT_EQ(success, Core::Status::CORE_OK);
            EXPECT_EQ(status, Led::LedState::ON);
        }

        TEST_F(UT_CMP_LED, Off) {
            Core::Status success = Core::Status::CORE_ERROR;

            EXPECT_CALL(mMockGpio, Reset()).WillRepeatedly(Return(Core::Status::CORE_OK));

            success = mLed.Off();

            const Led::LedState status = mLed.Get();

            EXPECT_EQ(success, Core::Status::CORE_OK);
            EXPECT_EQ(status, Led::LedState::OFF);
        }

        TEST_F(UT_CMP_LED, Toggle) {
            Core::Status success = Core::Status::CORE_ERROR;

            EXPECT_CALL(mMockGpio, Set()).WillOnce(Return(Core::Status::CORE_OK));

            success = mLed.Toggle();

            const Led::LedState status = mLed.Get();

            EXPECT_EQ(success, Core::Status::CORE_OK);
            EXPECT_EQ(status, Led::LedState::ON);
        }

        TEST_F(UT_CMP_LED, Get) {

            const Led::LedState status = mLed.Get();

            EXPECT_EQ(status, Led::LedState::OFF);
        }
    }
}
