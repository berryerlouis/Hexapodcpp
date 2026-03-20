#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockTwi.h"
#include "../../../mock/cmp/MockLed.h"
#include "../../../../src/Component/Proximity/Laser/Vl53l5x.h"

using ::testing::_;
using ::testing::StrictMock;


namespace Component
{
    namespace Proximity
    {
        namespace Laser
        {
            class UT_CMP_VL53L0X : public ::testing::Test {
            protected:
                UT_CMP_VL53L0X()
                    : mMockTwi()
                    , mVl53l5x(mMockTwi, mMockLed) {
                }

                virtual void SetUp() {
                }

                virtual void TearDown() {
                }

                virtual ~UT_CMP_VL53L0X() = default;

                /* Mocks */
                StrictMock<Component::Led::MockLed> mMockLed;
                StrictMock<Driver::Twi::MockTwi>    mMockTwi;

                /* Test class */
                                Vl53l5x mVl53l5x;
            };

            TEST_F(UT_CMP_VL53L0X, SetThreshold) {
                                EXPECT_EQ(Core::Status::CORE_OK, mVl53l5x.SetThreshold(350U));
                                EXPECT_EQ(350U, mVl53l5x.GetThreshold());
            }
        } // namespace Laser
    } // namespace Proximity
} // namespace Component
