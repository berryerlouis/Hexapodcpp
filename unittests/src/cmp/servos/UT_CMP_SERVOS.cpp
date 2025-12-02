#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockPca9685.h"
#include "../../../mock/drv/MockGpio.h"

#include "../../../../src/Component/Servos/Servos.h"

using ::testing::StrictMock;
using ::testing::_;
using ::testing::Return;

namespace Component
{
    namespace Servos
    {
        class UT_CMP_SERVOS : public ::testing::Test {
        protected:
            UT_CMP_SERVOS() :
                            mMockGpio()
                            , mMockPca9685_0()
                            , mMockPca9685_1()
                            , mServos(mMockPca9685_0, mMockPca9685_1, mMockGpio) {
            }

            virtual void
            SetUp() {
                Core::Status success = Core::Status::CORE_ERROR;

                EXPECT_CALL(mMockPca9685_0, GetAddress()).WillRepeatedly(Return(0U));
                EXPECT_CALL(mMockPca9685_1, GetAddress()).WillRepeatedly(Return(1U));
                EXPECT_CALL(mMockPca9685_0, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockPca9685_1, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, Set()).Times(1U);
                success = mServos.Initialize();
                EXPECT_EQ(success, Core::Status::CORE_OK);
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CMP_SERVOS() = default;

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio> mMockGpio;
            StrictMock<Component::ServosController::MockPca9685> mMockPca9685_0;
            StrictMock<Component::ServosController::MockPca9685> mMockPca9685_1;

            /* Test class */
            Servos mServos;
        };

        TEST_F(UT_CMP_SERVOS, Update_Ok) {
            for (size_t i = 0; i < NB_SERVOS; i++) {
                mServos.GetServo(static_cast<EServos>(i))->SetEnable(true);
            }

            EXPECT_CALL(mMockPca9685_0, SetPwm(_, _)).Times(NB_SERVOS / 2U);
            EXPECT_CALL(mMockPca9685_1, SetPwm(_, _)).Times(NB_SERVOS / 2U);

            mServos.Update(0UL);
        }
    }
}