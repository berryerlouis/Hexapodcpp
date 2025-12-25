#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSound.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

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
                mMockEventDispatcherInterface(),
                mMockMessageInterface(),
                mMockSoundLeft(),
                mMockSoundRight(),
                mServiceSound(mMockSoundLeft,
                              mMockSoundRight,
                              mMockMessageInterface,
                              mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_SOUND() = default;

            /* Mocks */
            StrictMock<Event::MockEventDispatcherInterface> mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>       mMockMessageInterface;
            StrictMock<Component::Sound::MockSound>         mMockSoundLeft;
            StrictMock<Component::Sound::MockSound>         mMockSoundRight;

            /* Test class */
            ServiceSound mServiceSound;
        };

        TEST_F(UT_SRV_SOUND,
               Initialize) {
            EXPECT_CALL(mMockSoundLeft, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockSoundRight, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_EQ(Core::Status::CORE_OK, mServiceSound.Initialize());
        }
    } // namespace Sound
} // namespace Service
