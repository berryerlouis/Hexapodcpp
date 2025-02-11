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
                EXPECT_CALL(mMockLed, Initialize()).Times(1U);
                EXPECT_CALL(mMockLed, Off()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                EXPECT_EQ(mSoundLeft.Initialize(), Core::Status::CORE_OK);
                EXPECT_EQ(mSoundLeft.GetStatus(), NO_SOUND);

                EXPECT_CALL(mMockLed, Initialize()).Times(1U);
                EXPECT_CALL(mMockLed, Off()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                EXPECT_EQ(mSoundRight.Initialize(), Core::Status::CORE_OK);
                EXPECT_EQ(mSoundRight.GetStatus(), NO_SOUND);
            }

            virtual void
            TearDown() {
                Sound::soundIndex = 0U;
            }

            virtual ~UT_CMP_SOUND() = default;

            void
            HitTest(const SoundId &soundId, const SoundState &soundState, const uint64_t time) {
                if (soundState == LOUD) {
                    EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(false));
                    EXPECT_CALL(mMockLed, On()).WillOnce(Return(Core::Status::CORE_OK));
                    EXPECT_CALL(mMockTick, GetUs()).Times(1U).WillOnce(Return(time));
                } else {
                    EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(true));
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
            HitTest(SOUND_LEFT, LOUD, 1U);
            HitTest(SOUND_LEFT, NO_SOUND, 55U);

            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);
            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 54U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 0U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Left_Stop_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 100U);
            HitTest(SOUND_RIGHT, LOUD, 110U);
            HitTest(SOUND_LEFT, NO_SOUND, 120U);

            mSoundLeft.Update(1000U);
            EXPECT_CALL(mMockTick, GetUs()).Times(1U).WillOnce(Return(1000U));
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundRight.GetStatus(), LOUD);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 20U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 890U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Right_Stop_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_RIGHT, NO_SOUND, 130U);

            EXPECT_CALL(mMockTick, GetUs()).Times(1U).WillOnce(Return(1000U));
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 890U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 10U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Left_Right_Stop_Right_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_RIGHT, NO_SOUND, 130U);
            HitTest(SOUND_LEFT, NO_SOUND, 140U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 10U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Exact_time_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, NO_SOUND, 140U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), LOUD);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Exact_time_2_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, LOUD, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            HitTest(SOUND_LEFT, NO_SOUND, 160U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), LOUD);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);
        }

        TEST_F(UT_CMP_SOUND, Hit_2_Update_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, LOUD, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            HitTest(SOUND_LEFT, NO_SOUND, 160U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), LOUD);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);

            mSoundLeft.Update(2000U);
            mSoundRight.Update(2000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundRight.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 0U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 0U);
        }

        TEST_F(UT_CMP_SOUND, Hit_4_Update_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, LOUD, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            HitTest(SOUND_LEFT, NO_SOUND, 160U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), LOUD);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);

            mSoundLeft.Update(2000U);
            mSoundRight.Update(2000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundRight.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 0U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 0U);

            HitTest(SOUND_RIGHT, LOUD, 3120U);
            HitTest(SOUND_LEFT, LOUD, 3130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 3150U);
            HitTest(SOUND_LEFT, NO_SOUND, 3160U);
            mSoundLeft.Update(3000U);
            mSoundRight.Update(3000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), LOUD);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);

            HitTest(SOUND_RIGHT, LOUD, 4120U);
            HitTest(SOUND_LEFT, LOUD, 4130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 4150U);
            HitTest(SOUND_LEFT, NO_SOUND, 4170U);
            mSoundLeft.Update(4000U);
            mSoundRight.Update(4000U);

            EXPECT_EQ(mSoundLeft.GetStatus(), LOUD);
            EXPECT_EQ(mSoundRight.GetStatus(), NO_SOUND);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 40U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);
        }
    }
}
