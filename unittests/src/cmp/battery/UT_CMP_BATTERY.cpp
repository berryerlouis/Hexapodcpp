#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../../src/Component/Battery/Battery.h"
#include "../../../mock/cmp/MockAds1115.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
    namespace Battery
    {
        class UT_CMP_BATTERY : public ::testing::Test {
        protected:
            UT_CMP_BATTERY()
                : mMockAdc()
                , mBattery(mMockAdc) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockAdc, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(mBattery.Initialize(), Core::Status::CORE_OK);
            }

            virtual void TearDown() {
            }

            virtual ~UT_CMP_BATTERY() = default;

            /* Mocks */
            StrictMock<Component::Adc::MockAds1115> mMockAdc;

            /* Test class */
            Battery mBattery;
        };


        TEST_F(UT_CMP_BATTERY, GetStateAfterInit) {
            BatteryState state = mBattery.GetState();
            EXPECT_EQ(state, BatteryState::UNKNOWN);
        }

        TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateCritical) {
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_0)).WillOnce(Return(740U));
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_1)).WillOnce(Return(740U));

            mBattery.Update(0UL);
            const BatteryState state = mBattery.GetState();
            EXPECT_EQ(state, BatteryState::CRITICAL);
        }

        TEST_F(UT_CMP_BATTERY, GetVoltageNominal) {
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_0)).WillOnce(Return(740U));
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_1)).WillOnce(Return(740U));

            mBattery.Update(0UL);
            const uint16_t voltage = mBattery.GetVoltage();
            EXPECT_EQ(voltage, (uint16_t) (740U * 0.46F));
        }

        TEST_F(UT_CMP_BATTERY, GetIntensityNominal) {
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_0)).WillOnce(Return(740U));
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_1)).WillOnce(Return(740U));

            mBattery.Update(0UL);
            const uint16_t intensity = mBattery.GetIntensity();
            EXPECT_EQ(intensity, (uint16_t) (740U * 0.066F));
        }

        TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateWarning) {
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_0))
                    .WillOnce(Return(799U / 0.46F));
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_1))
                    .WillOnce(Return(799U / 0.46F));

            mBattery.Update(0UL);
            const BatteryState state = mBattery.GetState();
            EXPECT_EQ(state, BatteryState::WARNING);
        }

        TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateNominal) {
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_0))
                    .WillOnce(Return(900U / 0.46F));
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_1))
                    .WillOnce(Return(900U / 0.46F));

            mBattery.Update(0UL);

            const BatteryState state = mBattery.GetState();
            EXPECT_EQ(state, BatteryState::NOMINAL);
        }

        TEST_F(UT_CMP_BATTERY, GetStateAfterUpdateNominalTwice) {
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_0))
                    .WillRepeatedly(Return(900U / 0.46F));
            EXPECT_CALL(mMockAdc, ReadADC(Adc::PIN_1))
                    .WillRepeatedly(Return(900U / 0.46F));

            mBattery.Update(0UL);
            mBattery.Update(0UL);
            const BatteryState state = mBattery.GetState();

            EXPECT_EQ(state, BatteryState::NOMINAL);
        }
    } // namespace Battery
} // namespace Component
