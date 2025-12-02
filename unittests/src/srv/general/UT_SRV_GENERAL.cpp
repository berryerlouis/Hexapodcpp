#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSoftware.h"
#include "../../../mock/srv/MockEventListener.h"

#include "../../../../src/Cluster/General/ClusterGeneral.h"
#include "../../../../src/Service/General/ServiceGeneral.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace General
    {
        class UT_SRV_GENERAL : public ::testing::Test {
        protected:
            UT_SRV_GENERAL() :
                             mMockSoftware()
                             , mMockEventListener()
                             , mServiceGeneral(mMockSoftware, mMockEventListener) {
            }

            virtual void
            SetUp() {
                EXPECT_CALL(mMockSoftware, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceGeneral.Initialize());

                EXPECT_CALL(mMockSoftware, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceGeneral.Initialize());
            }

            virtual void
            TearDown() {
            }

            virtual ~UT_SRV_GENERAL() = default;

            /* Mocks */
            StrictMock<Component::Software::MockSoftware> mMockSoftware;
            StrictMock<Event::MockEventListener> mMockEventListener;

            /* Test class */
            ServiceGeneral mServiceGeneral;
        };

        TEST_F(UT_SRV_GENERAL, Update_Ok) {
            EXPECT_CALL(mMockSoftware, Update(500U)).Times(1U);
            mServiceGeneral.Update(500U);
        }
    }
}