#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSound.h"
#include "../../../../src/Cluster/Constants.h"
#include "../../../../src/Cluster/Sound/ClusterSound.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Cluster
{
    namespace Sound
    {
        class UT_CLU_SOUND : public ::testing::Test {
        protected:
            UT_CLU_SOUND() :
                mMockSoundLeft(),
                mMockSoundRight(),
                mClusterSound(mMockSoundLeft, mMockSoundRight) {
            }

            virtual void
            SetUp() {
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_CLU_SOUND() = default;

            /* Mocks */
            StrictMock<Component::Sound::MockSound> mMockSoundLeft;
            StrictMock<Component::Sound::MockSound> mMockSoundRight;

            /* Test class */
            ClusterSound mClusterSound;
        };

        TEST_F(UT_CLU_SOUND, Execute_Command_status_Left_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame request(SOUND, GET_SOUND_STATUS);
            request.Set1ByteParam(SOUND_LEFT);
            Frame response;

            EXPECT_CALL(mMockSoundLeft, GetIntervalSoundHit()).WillOnce(Return(1110U));

            success = mClusterSound.ExecuteFrame(request, response);

            EXPECT_EQ(response.clusterId, SOUND);
            EXPECT_EQ(response.commandId, GET_SOUND_STATUS);
            EXPECT_EQ(response.nbParams, 9U);
            EXPECT_EQ(response.params[0U], SOUND_LEFT);
            EXPECT_EQ(response.params[1U], 0x56);
            EXPECT_EQ(response.params[2U], 0x04);
            EXPECT_EQ(response.params[3U], 0x00);
            EXPECT_EQ(response.params[4U], 0x00);
            EXPECT_EQ(response.params[5U], 0x00);
            EXPECT_EQ(response.params[6U], 0x00);
            EXPECT_EQ(response.params[7U], 0x00);
            EXPECT_EQ(response.params[8U], 0x00);
            EXPECT_EQ(response.params[9U], 0x00);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SOUND, Execute_Command_status_Right_Ok) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame request(SOUND, GET_SOUND_STATUS);
            request.Set1ByteParam(SOUND_RIGHT);
            Frame response;

            EXPECT_CALL(mMockSoundRight, GetIntervalSoundHit()).WillOnce(Return(1110U));

            success = mClusterSound.ExecuteFrame(request, response);

            EXPECT_EQ(response.clusterId, SOUND);
            EXPECT_EQ(response.commandId, GET_SOUND_STATUS);
            EXPECT_EQ(response.nbParams, 9U);
            EXPECT_EQ(response.params[0U], SOUND_RIGHT);
            EXPECT_EQ(response.params[1U], 0x56);
            EXPECT_EQ(response.params[2U], 0x04);
            EXPECT_EQ(response.params[3U], 0x00);
            EXPECT_EQ(response.params[4U], 0x00);
            EXPECT_EQ(response.params[5U], 0x00);
            EXPECT_EQ(response.params[6U], 0x00);
            EXPECT_EQ(response.params[7U], 0x00);
            EXPECT_EQ(response.params[8U], 0x00);
            EXPECT_EQ(response.params[9U], 0x00);
            EXPECT_EQ(success, Core::Status::CORE_OK);
        }

        TEST_F(UT_CLU_SOUND, Execute_WrongCommand_Ko) {
            Core::Status success = Core::Status::CORE_ERROR;
            Frame request;
            Frame response;

            request.Build(SOUND, 0x5FU);
            success = mClusterSound.ExecuteFrame(request, response);

            EXPECT_EQ(response.clusterId, 0U);
            EXPECT_EQ(response.commandId, 0U);
            EXPECT_EQ(response.nbParams, 0U);
            EXPECT_EQ(success, Core::Status::CORE_ERROR);
        }
    }
}
