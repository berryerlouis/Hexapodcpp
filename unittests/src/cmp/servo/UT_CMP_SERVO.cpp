#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"

#include "../../../../src/Component/Servo/Servo.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
    namespace Servo
    {
        class UT_CMP_SERVO : public ::testing::Test {
        protected:
            UT_CMP_SERVO()
                : mMockPca9685()
                , mServo(mMockPca9685, 0U, 90U, 0, 0U, 180U, false) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockPca9685, GetAddress()).WillOnce(Return(0U));
                const Core::Status success = mServo.Initialize();
                mServo.SetEnable(true);
                EXPECT_EQ(success, Core::Status::CORE_OK);
            }

            virtual void TearDown() {
            }

            virtual ~UT_CMP_SERVO() = default;

            /* Mocks */
            StrictMock<Component::ServosController::MockPca9685> mMockPca9685;

            /* Test class */
            Servo mServo;
        };

        TEST_F(UT_CMP_SERVO, Update_Ok) {
            EXPECT_CALL(mMockPca9685, SetPwm(0U, _)).Times(1U);
            mServo.Update(0UL);
        }

        TEST_F(UT_CMP_SERVO, UpdateSpeed_Ok) {
            mServo.SetAngle(10U, 1000U);
            EXPECT_CALL(mMockPca9685, SetPwm(0U, _)).Times(1U);
            mServo.Update(0UL);
            EXPECT_TRUE(mServo.IsMoving());
        }

        TEST_F(UT_CMP_SERVO, UpdateSpeedEnd_Ok) {
            mServo.SetAngle(10U, 1000U);
            EXPECT_CALL(mMockPca9685, SetPwm(0U, _)).Times(1U);
            mServo.Update(1000UL);
            EXPECT_FALSE(mServo.IsMoving());
        }

        TEST_F(UT_CMP_SERVO, SetAngle_Ok) {
            EXPECT_EQ(Core::Status::CORE_OK, mServo.SetAngle(10U));
            EXPECT_TRUE(mServo.IsMoving());
            EXPECT_EQ(mServo.GetAngle(), 10U);
        }

        TEST_F(UT_CMP_SERVO, SetAngleSpeed_Ok) {
            EXPECT_EQ(Core::Status::CORE_OK, mServo.SetAngle(10U, 1000U));
            EXPECT_TRUE(mServo.IsMoving());
            EXPECT_EQ(mServo.GetAngle(), 90U);
        }

        TEST_F(UT_CMP_SERVO, SetAngle_Reverse_Ok) {
            mServo.SetReverse(true);

            EXPECT_EQ(Core::Status::CORE_OK, mServo.SetAngle(70U));
            EXPECT_TRUE(mServo.IsMoving());
            EXPECT_EQ(mServo.GetAngle(), 70U);
        }

        TEST_F(UT_CMP_SERVO, SetAngle_AboveMax_Ko) {
            EXPECT_EQ(Core::Status::CORE_ERROR_MAX, mServo.SetAngle(190U));
            EXPECT_EQ(mServo.GetAngle(), 90U);
            EXPECT_FALSE(mServo.IsMoving());
        }

        TEST_F(UT_CMP_SERVO, SetAngle_BelowMin_Ko) {
            mServo.SetMin(50U);

            EXPECT_EQ(Core::Status::CORE_ERROR_MIN, mServo.SetAngle(40U));
            EXPECT_EQ(mServo.GetAngle(), 90U);
            EXPECT_FALSE(mServo.IsMoving());
        }

        TEST_F(UT_CMP_SERVO, SetAngle_SetOffset_Ok) {
            mServo.SetOffset(-10);

            EXPECT_EQ(Core::Status::CORE_OK, mServo.SetAngle(120U));
            EXPECT_EQ(mServo.GetAngle(), 120U);
            EXPECT_TRUE(mServo.IsMoving());
        }

        TEST_F(UT_CMP_SERVO, SetAngle_AboveMax_WithOffset_Ko) {
            mServo.SetOffset(-10);

            EXPECT_EQ(Core::Status::CORE_ERROR_MAX, mServo.SetAngle(190U));
            EXPECT_EQ(mServo.GetAngle(), 90U);
            EXPECT_FALSE(mServo.IsMoving());
        }

        TEST_F(UT_CMP_SERVO, SetAngle_BelowMin_WithOffset_Ko) {
            mServo.SetOffset(10);
            mServo.SetMin(50U);

            EXPECT_EQ(Core::Status::CORE_ERROR_MIN, mServo.SetAngle(40U));
            EXPECT_EQ(mServo.GetAngle(), 90U);
            EXPECT_FALSE(mServo.IsMoving());
        }

        TEST_F(UT_CMP_SERVO, SetMin_Ok) {
            EXPECT_TRUE(mServo.SetMin(50U));
            EXPECT_EQ(mServo.GetMin(), 50U);
        }

        TEST_F(UT_CMP_SERVO, SetMax_Ok) {
            EXPECT_TRUE(mServo.SetMax(90U));
            EXPECT_EQ(mServo.GetMax(), 90U);
        }

        TEST_F(UT_CMP_SERVO, SetMin_Nok) {
            mServo.SetMax(90U);
            EXPECT_FALSE(mServo.SetMin(200U));
        }

        TEST_F(UT_CMP_SERVO, SetMax_Nok) {
            EXPECT_FALSE(mServo.SetMax(200U));
        }

        TEST_F(UT_CMP_SERVO, IsEnable_Ok) {
            mServo.SetEnable(true);
            EXPECT_TRUE(mServo.IsEnable());
        }

        TEST_F(UT_CMP_SERVO, GetReverse_Ok) {
            mServo.SetReverse(true);
            EXPECT_TRUE(mServo.GetReverse());
        }

        TEST_F(UT_CMP_SERVO, SetEnableFalse_Ok) {
            EXPECT_CALL(mMockPca9685, SetPwm(0U, 4096U)).Times(1U);
            mServo.SetEnable(false);
        }
    } // namespace Servo
} // namespace Component
