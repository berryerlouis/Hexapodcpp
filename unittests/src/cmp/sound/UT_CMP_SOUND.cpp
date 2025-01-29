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
        class UT_CMP_SOUND : public ::testing::Test {
        protected:
            UT_CMP_SOUND() :
                mMockGpio(),
                mMockLed(),
                mMockTick(),
                mSoundLeft(SOUND_LEFT, mMockGpio, mMockLed, mMockTick),
                mSoundRight(SOUND_RIGHT, mMockGpio, mMockLed, mMockTick) {
            }

            virtual void
            SetUp() {
                Core::Status success = Core::Status::CORE_ERROR;
                EXPECT_CALL(mMockLed, Initialize()).Times(1U);
                EXPECT_CALL(mMockLed, Off()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                success = mSoundLeft.Initialize();
                EXPECT_EQ(success, Core::Status::CORE_OK);
                const SoundState status = mSoundLeft.Get();
                EXPECT_EQ(status, NO_SOUND);
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CMP_SOUND() = default;

            void
            HitTest(const SoundId &soundId, const SoundState &soundState, const uint64_t time) {
                if (soundState == LOUD) {
                    EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(true));
                    EXPECT_CALL(mMockLed, On()).WillOnce(Return(Core::Status::CORE_OK));
                    EXPECT_CALL(mMockTick, GetUs()).Times(1U).WillOnce(Return(time));
                } else {
                    EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(false));
                    EXPECT_CALL(mMockLed, Off()).WillOnce(Return(Core::Status::CORE_OK));
                    EXPECT_CALL(mMockTick, GetUs()).Times(1U).WillOnce(Return(time));
                }

                if (soundId == SOUND_LEFT) {
                    mSoundLeft.Hit();
                } else {
                    mSoundRight.Hit();
                }
            }

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio> mMockGpio;
            StrictMock<Component::Led::MockLed> mMockLed;
            StrictMock<Driver::Tick::MockTick> mMockTick;

            /* Test class */
            Sound mSoundLeft;
            Sound mSoundRight;
        };

        TEST_F(UT_CMP_SOUND, Hit_Left_Start_Ok) {
            HitTest(SOUND_LEFT, LOUD, 55U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_Ok) {
            HitTest(SOUND_LEFT, LOUD, 100U);
            HitTest(SOUND_RIGHT, LOUD, 110U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Left_Stop_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 100U);
            HitTest(SOUND_RIGHT, LOUD, 110U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_LEFT, NO_SOUND, 120U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Right_Stop_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_RIGHT, NO_SOUND, 130U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Left_Right_Stop_Right_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_RIGHT, NO_SOUND, 130U);
            HitTest(SOUND_LEFT, NO_SOUND, 140U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Left_Right_Stop_Left_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_LEFT, NO_SOUND, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 140U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Right_Stop_First_And_Right_Restart_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_LEFT, NO_SOUND, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 140U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_RIGHT, LOUD, 200U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_RIGHT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Right_Stop_First_And_Right_Restart_2_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_RIGHT, NO_SOUND, 130U);
            HitTest(SOUND_LEFT, NO_SOUND, 140U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_LEFT);
            HitTest(SOUND_RIGHT, LOUD, 200U);
            EXPECT_EQ(Sound::soundIdHit, SOUND_RIGHT);
        }
    }
}
