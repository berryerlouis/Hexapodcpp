#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/Component/Servo/Servo.h"

using ::testing::StrictMock;
using ::testing::_;
using ::testing::Return;

namespace Component
{
	namespace Servo
	{
		class UT_CMP_SERVO : public ::testing::Test {
		protected:
			UT_CMP_SERVO() : mMockTick(),
			                 mMockPca9685(),
			                 mServo(mMockPca9685, mMockTick, 0U, 90U, 0, 0U, 180U, false) {
			}

			virtual void SetUp() {
				const Core::CoreStatus success = mServo.Initialize();
				mServo.SetEnable(true);
				EXPECT_TRUE(success);
			}

			virtual void TearDown() {
			}

			virtual ~UT_CMP_SERVO() = default;

			/* Mocks */
			StrictMock<Driver::Tick::MockTick> mMockTick;
			StrictMock<Component::ServosController::MockPca9685> mMockPca9685;

			/* Test class */
			Servo mServo;
		};

		TEST_F(UT_CMP_SERVO, Update_Ok) {
			EXPECT_CALL(mMockPca9685, SetPwm( 0U, _ )).Times(0U);
			mServo.Update(0UL);
		}

		TEST_F(UT_CMP_SERVO, SetAngle_Ok) {
			EXPECT_TRUE(mServo.SetAngle( 10U ));
			EXPECT_TRUE(mServo.IsMoving());
			EXPECT_EQ(mServo.GetAngle(), 10U);
		}

		TEST_F(UT_CMP_SERVO, SetAngle_Reverse_Ok) {
			mServo.SetReverse(true);

			EXPECT_TRUE(mServo.SetAngle( 70U ));
			EXPECT_TRUE(mServo.IsMoving());
			EXPECT_EQ(mServo.GetAngle(), 70U);
		}

		TEST_F(UT_CMP_SERVO, SetAngle_AboveMax_Ko) {
			EXPECT_FALSE(mServo.SetAngle( 190U ));
			EXPECT_EQ(mServo.GetAngle(), 90U);
			EXPECT_FALSE(mServo.IsMoving());
		}

		TEST_F(UT_CMP_SERVO, SetAngle_BelowMin_Ko) {
			mServo.SetMin(50U);

			EXPECT_FALSE(mServo.SetAngle( 40U ));
			EXPECT_EQ(mServo.GetAngle(), 90U);
			EXPECT_FALSE(mServo.IsMoving());
		}

		TEST_F(UT_CMP_SERVO, SetAngle_SetOffset_Ok) {
			mServo.SetOffset(-10);

			EXPECT_TRUE(mServo.SetAngle( 120U ));
			EXPECT_EQ(mServo.GetAngle(), 120U);
			EXPECT_TRUE(mServo.IsMoving());
		}

		TEST_F(UT_CMP_SERVO, SetAngle_AboveMax_WithOffset_Ko) {
			mServo.SetOffset(-10); //max 170

			EXPECT_FALSE(mServo.SetAngle( 180U ));
			EXPECT_EQ(mServo.GetAngle(), 90U);
			EXPECT_FALSE(mServo.IsMoving());
		}

		TEST_F(UT_CMP_SERVO, SetAngle_BelowMin_WithOffset_Ko) {
			mServo.SetOffset(10);
			mServo.SetMin(50U); //min 60

			EXPECT_FALSE(mServo.SetAngle( 50U ));
			EXPECT_EQ(mServo.GetAngle(), 90U);
			EXPECT_FALSE(mServo.IsMoving());
		}

		TEST_F(UT_CMP_SERVO, SetMin_Ok) {
			EXPECT_TRUE(mServo.SetMin( 50U ));
			EXPECT_EQ(mServo.GetMin(), 50U);
		}

		TEST_F(UT_CMP_SERVO, SetMax_Ok) {
			EXPECT_TRUE(mServo.SetMax( 90U ));
			EXPECT_EQ(mServo.GetMax(), 90U);
		}
	}
}
