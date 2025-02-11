#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/Component/Adc/Ads1115.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
    namespace Adc
    {
        class UT_CMP_ADS1115 : public ::testing::Test {
        protected:
            UT_CMP_ADS1115() :
                mMockTwi(),
                mAds1115(mMockTwi) {
            }

            virtual void
            SetUp() {
                EXPECT_EQ(mAds1115.Initialize(), Core::Status::CORE_OK);
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CMP_ADS1115() = default;

            /* Mocks */
            StrictMock<Driver::Twi::MockTwi> mMockTwi;

            /* Test class */
            Ads1115 mAds1115;
        };

        TEST_F(UT_CMP_ADS1115, Update_Ok) {
            mAds1115.Update(0UL);
        }

        /*TEST_F(UT_CMP_ADS1115, GetStateAfterUpdateNominalTwice) {
            EXPECT_CALL(mMockTwi, GetVoltage()).WillRepeatedly(Return(900U));

            mAds1115.Update(0UL);

            EXPECT_EQ(state, BatteryState::NOMINAL);
        }*/
    }
}
