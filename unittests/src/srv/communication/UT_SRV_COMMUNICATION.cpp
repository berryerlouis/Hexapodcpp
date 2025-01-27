#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockCommunication.h"
#include "../../../mock/clu/MockClusters.h"
#include "../../../mock/srv/MockEventListener.h"

#include "../../../../src/Service/Communication/ServiceCommunication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Communication
    {
        class UT_SRV_COMMUNICATION : public ::testing::Test {
        protected:
            UT_SRV_COMMUNICATION() :
                mMockCommunication(),
                mMockClusters(),
                mMockEventListener(),
                mServiceCommunication(mMockCommunication, mMockClusters, mMockEventListener) {
            }

            virtual void
            SetUp() {
                EXPECT_CALL(mMockCommunication, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceCommunication.Initialize());

                EXPECT_CALL(mMockCommunication, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceCommunication.Initialize());
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_SRV_COMMUNICATION() = default;


            /* Mocks */
            StrictMock<Component::Communication::MockCommunication> mMockCommunication;
            StrictMock<Cluster::Clusters::MockClusters> mMockClusters;
            StrictMock<Event::MockEventListener> mMockEventListener;

            /* Test class */
            ServiceCommunication mServiceCommunication;
        };

        TEST_F(UT_SRV_COMMUNICATION, Initialize_Update) {
            EXPECT_CALL(mMockCommunication, Update( 12450UL )).Times(1U);
            mServiceCommunication.Update(12450UL);
        }

    }
}
