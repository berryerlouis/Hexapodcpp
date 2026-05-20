#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockBody.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Cluster/Body/ClusterBody.h"
#include "../../../../src/Service/Body/ServiceBody.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Body
    {
        class UT_SRV_BODY : public ::testing::Test {
        protected:
            UT_SRV_BODY()
                : mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mMockBody()
                , mServiceBody(mMockBody, mMockMessageInterface, mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockBody, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceBody.Initialize());

                EXPECT_CALL(mMockBody, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceBody.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_BODY() = default;

            /* Mocks */
            StrictMock<Event::MockEventDispatcherInterface> mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>       mMockMessageInterface;
            StrictMock<::Bot::Body::MockBody>               mMockBody;

            /* Test class */
            ServiceBody mServiceBody;
        };

        TEST_F(UT_SRV_BODY, Update) {
            EXPECT_CALL(mMockBody, Update(12340UL)).Times(1U);

            mServiceBody.Update(12340UL);
        }
    } // namespace Body
} // namespace Service
