#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/Component/Adc/Ads1115.h"

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArrayArgument;
using ::testing::StrictMock;

namespace Component
{
    namespace Adc
    {
        class UT_CMP_ADS1115 : public ::testing::Test {
        protected:
            UT_CMP_ADS1115()
                : mMockTwi()
                , mAds1115(mMockTwi) {
            }

            virtual void SetUp() {
                EXPECT_EQ(mAds1115.Initialize(), Core::Status::CORE_OK);
            }

            virtual void TearDown() {
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

        TEST_F(UT_CMP_ADS1115, ReadADC_Ok) {
            uint8_t buffer[2U] = {0x80U, 0x00U};

            for (size_t i = 0U; i < 4U; i++) {
                EXPECT_CALL(mMockTwi, WriteRegisters(_, ADS1115_REG_POINTER_CONFIG, _, 2U))
                        .WillOnce(Return(true));
                EXPECT_CALL(mMockTwi, ReadRegisters(_, ADS1115_REG_POINTER_CONFIG, _, 2U))
                        .WillOnce(DoAll(SetArrayArgument<2U>(buffer, buffer + 2U), Return(true)));
                EXPECT_CALL(mMockTwi, ReadRegisters(_, ADS1115_REG_POINTER_CONVERT, _, 2U))
                        .WillOnce(DoAll(SetArrayArgument<2U>(buffer, buffer + 2U), Return(true)));

                EXPECT_EQ(6144.0F, mAds1115.ReadADC((Ads1115Pin) i));
            }
        }

        /*TEST_F(UT_CMP_ADS1115,
        GetStateAfterUpdateNominalTwice) {
            EXPECT_CALL(mMockTwi,
        GetVoltage()).WillRepeatedly(Return(900U));

            mAds1115.Update(0UL);

            EXPECT_EQ(state, BatteryState::NOMINAL);
        }*/
    } // namespace Adc
} // namespace Component
