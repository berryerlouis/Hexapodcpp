#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockAdc.h"
#include "../../../../src/Component/Battery/Battery.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
	namespace Battery
	{
		class UT_CMP_BATTERY : public ::testing::Test {
		protected:
			UT_CMP_BATTERY() : mMockAdc(),
			                   mBattery(mMockAdc) {
			}

			virtual void SetUp() {
				EXPECT_CALL(mMockAdc, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_EQ(mBattery.Initialize(), Core::CoreStatus::CORE_OK);
			}

			virtual void TearDown() {
			}

			virtual ~UT_CMP_BATTERY() = default;

			/* Mocks */
			StrictMock<Driver::Adc::MockAdc> mMockAdc;

			/* Test class */
			Battery mBattery;
		};


		TEST_F(UT_CMP_BATTERY, GetStateAfterInit) {
			BatteryState state = mBattery.GetState();
			EXPECT_EQ(state, BatteryState::UNKNOWN);
		}


		TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateCritical) {
			EXPECT_CALL(mMockAdc, Read()).WillOnce(Return(74U));

			mBattery.Update(0UL);
			BatteryState state = mBattery.GetState();

			EXPECT_EQ(state, BatteryState::CRITICAL);
		}

		TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateWarning) {
			EXPECT_CALL(mMockAdc, Read()).WillOnce(Return(79U));

			mBattery.Update(0UL);
			BatteryState state = mBattery.GetState();

			EXPECT_EQ(state, BatteryState::WARNING);
		}

		TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateNominal) {
			EXPECT_CALL(mMockAdc, Read()).WillOnce(Return(90U));

			mBattery.Update(0UL);
			BatteryState state = mBattery.GetState();

			EXPECT_EQ(state, BatteryState::NOMINAL);
		}

		TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateNominalTwice) {
			EXPECT_CALL(mMockAdc, Read()).WillRepeatedly(Return(90U));

			mBattery.Update(0UL);
			mBattery.Update(0UL);
			BatteryState state = mBattery.GetState();

			EXPECT_EQ(state, BatteryState::NOMINAL);
		}
	}
}
