#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/drv/MockGpio.h"
#include "../../../mock/drv/MockTick.h"
#include "../../../mock/cmp/MockLed.h"
#include "../../../../src/Component/Sound/Sound.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
    namespace Sound
    {
        class UT_CMP_Sound : public ::testing::Test {
        protected:
            UT_CMP_Sound() :
                mMockGpio(),
                mMockLed(),
                mMockTick(),
                mSound(SOUND_LEFT, mMockGpio, mMockLed, mMockTick) {
            }

            virtual void
            SetUp() {
                Core::Status success = Core::Status::CORE_ERROR;
                EXPECT_CALL(mMockLed, Initialize()).Times(1U);
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                success = mSound.Initialize();
                EXPECT_EQ(success, Core::Status::CORE_OK);
                const SoundState status = mSound.Get();
                EXPECT_EQ(status, NO_SOUND);
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CMP_Sound() = default;

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio> mMockGpio;
            StrictMock<Component::Led::MockLed> mMockLed;
            StrictMock<Driver::Tick::MockTick> mMockTick;

            /* Test class */
            Sound mSound;
        };
    }
}
