#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockBody.h"
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
                , mMockLed()
                , mClusterBody(mMockBody)
                , mCommunication(mMockSocket, mMockLed) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockLed, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mCommunication.Initialize());
                EXPECT_CALL(mMockLed, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_OK, mCommunication.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_CMP_COMMUNICATION() = default;

            /* Mocks */
            StrictMock<Bot::Body::MockBody>             mMockBody;
            StrictMock<Driver::Socket::MockSocket>      mMockSocket;
            StrictMock<Component::Led::MockLed>         mMockLed;
            Cluster::Body::ClusterBody                  mClusterBody;

            /* Test class */
            Communication mCommunication;
        };

        TEST_F(UT_CMP_COMMUNICATION, Update_Ok_Noframe) {
            EXPECT_CALL(mMockSocket, Update(0U));
            EXPECT_CALL(mMockSocket, HasNewFrame()).WillOnce(Return(false));
            // EXPECT_CALL(mMockLed, On()).Times(0U);
            // EXPECT_CALL(mMockLed, Off()).Times(0U);
            mCommunication.Update(0UL);
        }

        TEST_F(UT_CMP_COMMUNICATION, Notified) {
            const Socket::SocketStruct state(Socket::SocketStruct::CLIENT_CONNECTED);
            mCommunication.Notified(state);
        }
    } // namespace Communication
} // namespace Component
