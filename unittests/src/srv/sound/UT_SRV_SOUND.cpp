#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSound.h"
#include "../../../mock/srv/MockEventListener.h"

#include "../../../../src/Cluster/Sound/ClusterSound.h"
#include "../../../../src/Service/Sound/ServiceSound.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Sound
    {
        class UT_SRV_SOUND : public ::testing::Test {
        protected:
            UT_SRV_SOUND() :
                mMockEventListener(),
                mMockSoundLeft(),
                mMockSoundRight(),
                mServiceSound(mMockSoundLeft, mMockSoundRight, mMockEventListener) {
            }

            virtual void
            SetUp() {
                EXPECT_CALL(mMockSoundLeft, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_CALL(mMockSoundRight, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceSound.Initialize());

                EXPECT_CALL(mMockSoundLeft, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockSoundRight, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockSoundLeft, Attach( _ )).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockSoundRight, Attach( _ )).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceSound.Initialize());
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_SRV_SOUND() = default;

            /* Mocks */
            StrictMock<Event::MockEventListener> mMockEventListener;
            StrictMock<Component::Sound::MockSound> mMockSoundLeft;
            StrictMock<Component::Sound::MockSound> mMockSoundRight;

            /* Test class */
            ServiceSound mServiceSound;
        };

        TEST_F(UT_SRV_SOUND, Update) {
            EXPECT_CALL(mMockSoundLeft, Update( 12340UL )).Times(1U);
            EXPECT_CALL(mMockSoundRight, Update( 12340UL )).Times(1U);
            mServiceSound.Update(12340UL);
        }

        TEST_F(UT_SRV_SOUND, UpdatedSoundState) {
            constexpr SoundState soundState = SoundState::NO_SOUND;

            Frame response;
            Cluster::Sound::ClusterSound::BuildFrameGetSoundState(SOUND_LEFT, soundState, response);
            EXPECT_CALL(mMockEventListener, SendMessage(response)).Times(1U);
            mServiceSound.UpdatedSoundState(SOUND_LEFT, soundState, 10U);
        }
    }
}
