#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockBody.h"
#include "../../../mock/clu/MockClusters.h"
#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/cmp/MockSoftware.h"
#include "../../../mock/drv/MockSocket.h"


#include "../../../../src/Cluster/Body/ClusterBody.h"
#include "../../../../src/Component/Communication/Communication.h"
#include "../../../../src/Component/Communication/Decoding/Protocol.h"
#include "../../../../src/Misc/Maths/Geometry.h"

using ::testing::_;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component
{
    namespace Communication
    {
        class UT_CMP_COMMUNICATION : public ::testing::Test {
        protected:
            UT_CMP_COMMUNICATION()
                : mMockBody()
                , mMockSocket()
                , mMockClusters()
                , mMockLed()
                , mClusterBody(mMockBody)
                , mCommunication(mMockSocket, mMockClusters, mMockLed) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockLed, Initialize())
                        .WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR,
                          mCommunication.Initialize());
                EXPECT_CALL(mMockLed, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_OK, mCommunication.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_CMP_COMMUNICATION() = default;

            /* Mocks */
            StrictMock<Bot::Body::MockBody>             mMockBody;
            StrictMock<Driver::Socket::MockSocket>      mMockSocket;
            StrictMock<Cluster::Clusters::MockClusters> mMockClusters;
            StrictMock<Component::Led::MockLed>         mMockLed;
            Cluster::Body::ClusterBody                  mClusterBody;

            /* Test class */
            Communication mCommunication;
        };

        TEST_F(UT_CMP_COMMUNICATION, Update_Ok_Noframe) {
            EXPECT_CALL(mMockSocket, Update(0U));
            EXPECT_CALL(mMockSocket, DataAvailable()).WillOnce(Return(0U));
            // EXPECT_CALL(mMockLed, On()).Times(0U);
            // EXPECT_CALL(mMockLed, Off()).Times(0U);
            mCommunication.Update(0UL);
        }

        TEST_F(UT_CMP_COMMUNICATION, Notified) {
            const Socket::SocketStruct state(
                    Socket::SocketStruct::CLIENT_CONNECTED);
            mCommunication.Notified(state);
        }

        TEST_F(UT_CMP_COMMUNICATION, Update_Ok_1frame_with_unknown_char) {
            const char *bufferRx = "<00z000>";
            EXPECT_CALL(mMockSocket, Update(0U));
            // EXPECT_CALL(mMockLed, On()).Times(0U);
            // EXPECT_CALL(mMockLed, Off()).Times(0U);
            EXPECT_CALL(mMockSocket, DataAvailable())
                    .WillOnce(Return(strlen(bufferRx)));

            ::testing::Sequence s;
            for (size_t i = 0U; i < strlen(bufferRx); i++) {
                EXPECT_CALL(mMockSocket, Read())
                        .InSequence(s)
                        .WillOnce(Return(bufferRx[i]));
            }

            mCommunication.Update(0UL);
        }

        TEST_F(UT_CMP_COMMUNICATION, Update_Ok_1frame) {
            const char *bufferRx = "<000000>";

            EXPECT_CALL(mMockSocket, Update(0U));
            // EXPECT_CALL(mMockLed,
            // On()).WillOnce(Return(Core::Status::CORE_OK));
            // EXPECT_CALL(mMockLed,
            // Off()).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockSocket, Send(Matcher<const char *>(_), _))
                    .Times(1U);
            EXPECT_CALL(mMockClusters, GetCluster(GENERAL)).Times(1U);

            EXPECT_CALL(mMockSocket, DataAvailable())
                    .WillOnce(Return(strlen(bufferRx)));
            ::testing::Sequence s;
            for (size_t i = 0U; i < strlen(bufferRx); i++) {
                EXPECT_CALL(mMockSocket, Read())
                        .InSequence(s)
                        .WillOnce(Return(bufferRx[i]));
            }
            mCommunication.Update(0UL);
        }

        TEST_F(UT_CMP_COMMUNICATION, Update_Ok_1frame_IMU) {
            const char *bufferRx = "<010000>";

            EXPECT_CALL(mMockSocket, Update(0U));
            // EXPECT_CALL(mMockLed,
            // On()).WillOnce(Return(Core::Status::CORE_OK));
            // EXPECT_CALL(mMockLed,
            // Off()).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockSocket, Send(Matcher<const char *>(_), _))
                    .Times(1U);
            EXPECT_CALL(mMockClusters, GetCluster(IMU)).Times(1U);

            EXPECT_CALL(mMockSocket, DataAvailable())
                    .WillOnce(Return(strlen(bufferRx)));
            ::testing::Sequence s;
            for (size_t i = 0U; i < strlen(bufferRx); i++) {
                EXPECT_CALL(mMockSocket, Read())
                        .InSequence(s)
                        .WillOnce(Return(bufferRx[i]));
            }
            mCommunication.Update(0UL);
        }

        TEST_F(UT_CMP_COMMUNICATION, Update_Ko_1frame) {
            const char *bufferRx = "<0<0000>";

            EXPECT_CALL(mMockSocket, Update(0U));

            EXPECT_CALL(mMockSocket, DataAvailable())
                    .WillOnce(Return(strlen(bufferRx)));
            ::testing::Sequence s;
            for (size_t i = 0U; i < strlen(bufferRx); i++) {
                EXPECT_CALL(mMockSocket, Read())
                        .InSequence(s)
                        .WillOnce(Return(bufferRx[i]));
            }
            mCommunication.Update(0UL);
        }

        TEST_F(UT_CMP_COMMUNICATION, Update_Ko_Cluster_not_found) {
            const char *bufferRx = "<1F0000>";

            EXPECT_CALL(mMockSocket, Update(0U));

            EXPECT_CALL(mMockSocket, DataAvailable())
                    .WillOnce(Return(strlen(bufferRx)));

            EXPECT_CALL(mMockSocket, Send(Matcher<const char *>(_), _))
                    .Times(1U);
            ::testing::Sequence s;
            for (size_t i = 0U; i < strlen(bufferRx); i++) {
                EXPECT_CALL(mMockSocket, Read())
                        .InSequence(s)
                        .WillOnce(Return(bufferRx[i]));
            }
            mCommunication.Update(0UL);
        }

        TEST_F(UT_CMP_COMMUNICATION, Update_Ko_Parse_Status) {
            const char *bufferRx = "<010001>";

            EXPECT_CALL(mMockSocket, Update(0U));

            EXPECT_CALL(mMockSocket, DataAvailable())
                    .WillOnce(Return(strlen(bufferRx)));

            EXPECT_CALL(mMockSocket, Send(Matcher<const char *>(_), _))
                    .Times(1U);
            ::testing::Sequence s;
            for (size_t i = 0U; i < strlen(bufferRx); i++) {
                EXPECT_CALL(mMockSocket, Read())
                        .InSequence(s)
                        .WillOnce(Return(bufferRx[i]));
            }
            mCommunication.Update(0UL);
        }


        TEST_F(UT_CMP_COMMUNICATION, Update_Cluster_Null) {
            const char *bufferRx = "<050000>";

            EXPECT_CALL(mMockSocket, Update(0U));
            // EXPECT_CALL(mMockLed,
            // On()).WillOnce(Return(Core::Status::CORE_OK));
            // EXPECT_CALL(mMockLed,
            // Off()).WillOnce(Return(Core::Status::CORE_OK));
            EXPECT_CALL(mMockSocket, Send(Matcher<const char *>(_), _))
                    .Times(1U);
            EXPECT_CALL(mMockClusters, GetCluster(BODY))
                    .Times(1U)
                    .WillOnce(Return(&mClusterBody));

            EXPECT_CALL(mMockBody, GetAmplitude())
                    .Times(1U)
                    .WillOnce(Return(0U));
            EXPECT_CALL(mMockBody, GetElevation())
                    .Times(1U)
                    .WillOnce(Return(0U));
            EXPECT_CALL(mMockBody, GetDirection())
                    .Times(1U)
                    .WillOnce(Return(0U));
            EXPECT_CALL(mMockBody, GetDuration())
                    .Times(1U)
                    .WillOnce(Return(0U));
            EXPECT_CALL(mMockBody, GetRotation())
                    .Times(1U)
                    .WillOnce(Return(0U));
            EXPECT_CALL(mMockBody, GetRotationClockWize())
                    .Times(1U)
                    .WillOnce(Return(0U));

            EXPECT_CALL(mMockSocket, DataAvailable())
                    .WillOnce(Return(strlen(bufferRx)));

            ::testing::Sequence s;
            for (size_t i = 0U; i < strlen(bufferRx); i++) {
                EXPECT_CALL(mMockSocket, Read())
                        .InSequence(s)
                        .WillOnce(Return(bufferRx[i]));
            }
            mCommunication.Update(0UL);
        }
    } // namespace Communication
} // namespace Component
