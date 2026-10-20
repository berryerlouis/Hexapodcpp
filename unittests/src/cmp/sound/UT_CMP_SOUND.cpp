#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../../src/Component/Sound/Sound.h"
#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/drv/MockGpio.h"

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;

namespace Component
{
    namespace Sound
    {
        class UT_CMP_SOUND : public ::testing::Test {
        protected:
            UT_CMP_SOUND()
                : mMockGpio()
                , mMockLed()
                , mSoundLeft(SOUND_LEFT, mMockGpio, mMockLed)
                , mSoundRight(SOUND_RIGHT, mMockGpio, mMockLed) {
            }

            enum SoundState {
                LOUD = 0x00U,
                NO_SOUND
            };

            virtual void SetUp() {
                Driver::Gpio::SGpio gpio;
                EXPECT_CALL(mMockGpio, GetPin()).WillOnce(ReturnRef(gpio));
                EXPECT_CALL(mMockLed, Initialize()).Times(1U);
                EXPECT_CALL(mMockLed, Off()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                EXPECT_EQ(mSoundLeft.Initialize(), Core::Status::CORE_OK);

                EXPECT_CALL(mMockGpio, GetPin()).WillOnce(ReturnRef(gpio));
                EXPECT_CALL(mMockLed, Initialize()).Times(1U);
                EXPECT_CALL(mMockLed, Off()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockGpio, SetInterruptPin(_)).Times(1U);
                EXPECT_EQ(mSoundRight.Initialize(), Core::Status::CORE_OK);
            }

            virtual void TearDown() {
                Sound::soundIndex = 0U;
            }

            virtual ~UT_CMP_SOUND() = default;

            void
            HitTest(const SoundId &soundId, const SoundState &soundState, const uint64_t time) {
                Driver::Timer::Tick::GetInstance().SetUs(time);
                if (soundState == LOUD) {
                    EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(false));
                    EXPECT_CALL(mMockLed, On()).WillOnce(Return(Core::Status::CORE_OK));
                } else {
                    EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(true));
                    EXPECT_CALL(mMockLed, Off()).WillOnce(Return(Core::Status::CORE_OK));
                }

                if (soundId == SOUND_LEFT) {
                    mSoundLeft.Hit();
                } else {
                    mSoundRight.Hit();
                }
            }

            /* Mocks */
            StrictMock<Driver::Gpio::MockGpio>  mMockGpio;
            StrictMock<Component::Led::MockLed> mMockLed;

            /* Test class */
            Sound mSoundLeft;
            Sound mSoundRight;
        };

        TEST_F(UT_CMP_SOUND, Hit_Left_Start_Ok) {
            HitTest(SOUND_LEFT, LOUD, 1U);
            HitTest(SOUND_LEFT, NO_SOUND, 55U);

            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);
            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 54U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 0U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Left_Stop_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 100U);
            HitTest(SOUND_RIGHT, LOUD, 110U);
            HitTest(SOUND_LEFT, NO_SOUND, 120U);

            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 20U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 890U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Right_Stop_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_RIGHT, NO_SOUND, 130U);

            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 890U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 10U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Start_And_Left_Right_Stop_Right_First_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_RIGHT, NO_SOUND, 130U);
            HitTest(SOUND_LEFT, NO_SOUND, 140U);
            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 10U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Exact_time_Ok) {
            HitTest(SOUND_LEFT, LOUD, 110U);
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, NO_SOUND, 140U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);
        }

        TEST_F(UT_CMP_SOUND, Hit_Left_Right_Exact_time_2_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, LOUD, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            HitTest(SOUND_LEFT, NO_SOUND, 160U);
            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);
        }

        TEST_F(UT_CMP_SOUND, Hit_2_Update_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, LOUD, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            HitTest(SOUND_LEFT, NO_SOUND, 160U);
            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);

            Driver::Timer::Tick::GetInstance().SetUs(2000U);
            mSoundLeft.Update(2000U);
            mSoundRight.Update(2000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);
        }

        TEST_F(UT_CMP_SOUND, Hit_4_Update_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 120U);
            HitTest(SOUND_LEFT, LOUD, 130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 150U);
            HitTest(SOUND_LEFT, NO_SOUND, 160U);
            Driver::Timer::Tick::GetInstance().SetUs(1000U);
            mSoundLeft.Update(1000U);
            mSoundRight.Update(1000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);

            Driver::Timer::Tick::GetInstance().SetUs(2000U);
            mSoundLeft.Update(2000U);
            mSoundRight.Update(2000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);

            HitTest(SOUND_RIGHT, LOUD, 3120U);
            HitTest(SOUND_LEFT, LOUD, 3130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 3150U);
            HitTest(SOUND_LEFT, NO_SOUND, 3160U);

            Driver::Timer::Tick::GetInstance().SetUs(3000U);
            mSoundLeft.Update(3000U);
            mSoundRight.Update(3000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 30U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);

            HitTest(SOUND_RIGHT, LOUD, 4120U);
            HitTest(SOUND_LEFT, LOUD, 4130U);
            HitTest(SOUND_RIGHT, NO_SOUND, 4150U);
            HitTest(SOUND_LEFT, NO_SOUND, 4170U);

            Driver::Timer::Tick::GetInstance().SetUs(4000U);
            mSoundLeft.Update(4000U);
            mSoundRight.Update(4000U);

            EXPECT_EQ(mSoundLeft.GetIntervalSoundHit(), 40U);
            EXPECT_EQ(mSoundRight.GetIntervalSoundHit(), 30U);
        }


        TEST_F(UT_CMP_SOUND, ComputeAndNotifyMaxSound_Ok) {

            HitTest(SOUND_RIGHT, LOUD, 4120U);
            HitTest(SOUND_LEFT, LOUD, 4130U);
            mSoundRight.ComputeAndNotifyMaxSound();
        }

        TEST_F(UT_CMP_SOUND, ComputeAndNotifyMaxSound_Left_First_Arrival_Wins_Over_Duration) {
            HitTest(SOUND_LEFT, LOUD, 1000U);
            HitTest(SOUND_RIGHT, LOUD, 1300U);
            HitTest(SOUND_LEFT, NO_SOUND, 3000U);
            HitTest(SOUND_RIGHT, NO_SOUND, 5000U);
            mSoundLeft.Update(10000U);
            mSoundRight.Update(10000U);

            const SoundStruct result = Sound::ComputeAndNotifyMaxSound();
            EXPECT_EQ(result.id, SOUND_LEFT);
            EXPECT_EQ(result.delay, 2000U);
        }

        TEST_F(UT_CMP_SOUND, ComputeAndNotifyMaxSound_Right_First_Arrival_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 1000U);
            HitTest(SOUND_LEFT, LOUD, 1200U);
            HitTest(SOUND_RIGHT, NO_SOUND, 3000U);
            HitTest(SOUND_LEFT, NO_SOUND, 3200U);
            mSoundLeft.Update(10000U);
            mSoundRight.Update(10000U);

            const SoundStruct result = Sound::ComputeAndNotifyMaxSound();
            EXPECT_EQ(result.id, SOUND_RIGHT);
            EXPECT_EQ(result.delay, 2000U);
        }

        TEST_F(UT_CMP_SOUND, ComputeAndNotifyMaxSound_Close_Arrival_Longest_Wins) {
            HitTest(SOUND_LEFT, LOUD, 1000U);
            HitTest(SOUND_RIGHT, LOUD, 1010U);
            HitTest(SOUND_LEFT, NO_SOUND, 3000U);
            HitTest(SOUND_RIGHT, NO_SOUND, 5000U);
            mSoundLeft.Update(10000U);
            mSoundRight.Update(10000U);

            const SoundStruct result = Sound::ComputeAndNotifyMaxSound();
            EXPECT_EQ(result.id, SOUND_RIGHT);
            EXPECT_EQ(result.delay, 3990U);
        }

        TEST_F(UT_CMP_SOUND, ComputeAndNotifyMaxSound_Single_Sensor_Ok) {
            HitTest(SOUND_RIGHT, LOUD, 1000U);
            HitTest(SOUND_RIGHT, NO_SOUND, 4000U);
            mSoundLeft.Update(10000U);
            mSoundRight.Update(10000U);

            const SoundStruct result = Sound::ComputeAndNotifyMaxSound();
            EXPECT_EQ(result.id, SOUND_RIGHT);
            EXPECT_EQ(result.delay, 3000U);
        }

        TEST_F(UT_CMP_SOUND, ComputeAndNotifyMaxSound_Noise_Ignored) {
            HitTest(SOUND_LEFT, LOUD, 1000U);
            HitTest(SOUND_LEFT, NO_SOUND, 1200U);
            mSoundLeft.Update(10000U);
            mSoundRight.Update(10000U);

            EXPECT_EQ(Sound::ComputeAndNotifyMaxSound().id, SOUND_NONE);
        }

        TEST_F(UT_CMP_SOUND, ComputeAndNotifyMaxSound_Majority_Of_Events_Wins) {
            HitTest(SOUND_LEFT, LOUD, 10000U);
            HitTest(SOUND_RIGHT, LOUD, 10300U);
            HitTest(SOUND_LEFT, NO_SOUND, 12000U);
            HitTest(SOUND_RIGHT, NO_SOUND, 12300U);
            HitTest(SOUND_RIGHT, LOUD, 20000U);
            HitTest(SOUND_LEFT, LOUD, 20300U);
            HitTest(SOUND_RIGHT, NO_SOUND, 22000U);
            HitTest(SOUND_LEFT, NO_SOUND, 22300U);
            HitTest(SOUND_RIGHT, LOUD, 30000U);
            HitTest(SOUND_LEFT, LOUD, 30300U);
            HitTest(SOUND_RIGHT, NO_SOUND, 32000U);
            HitTest(SOUND_LEFT, NO_SOUND, 32300U);
            mSoundLeft.Update(40000U);
            mSoundRight.Update(40000U);

            EXPECT_EQ(Sound::ComputeAndNotifyMaxSound().id, SOUND_RIGHT);
        }

        TEST_F(UT_CMP_SOUND, Hit_Without_Level_Change_Is_Ignored) {
            HitTest(SOUND_LEFT, LOUD, 1000U);
            EXPECT_CALL(mMockGpio, Get()).WillOnce(Return(false));
            mSoundLeft.Hit(5000U);
            EXPECT_EQ(mSoundLeft.mStartSoundTime, 1000U);
        }


    } // namespace Sound
} // namespace Component
