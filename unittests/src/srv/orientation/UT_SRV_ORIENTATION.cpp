#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockBarometer.h"
#include "../../../mock/cmp/MockMpu9150.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"
#include "../../../mock/srv/MockService.h"

#include "../../../../src/Cluster/Imu/ClusterImu.h"
#include "../../../../src/Service/Orientation/ServiceOrientation.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Orientation
    {
        class UT_SRV_ORIENTATION : public ::testing::Test {
        protected:
            UT_SRV_ORIENTATION()
                : mMockMpu9150()
                , mMockBarometer()
                , mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mServiceOrientation(mMockMpu9150,
                                      mMockBarometer,
                                      mMockMessageInterface,
                                      mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockMpu9150, Initialize())
                        .WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_CALL(mMockBarometer, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_EQ(Core::Status::CORE_ERROR,
                          mServiceOrientation.Initialize());

                EXPECT_CALL(mMockMpu9150, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockBarometer, Initialize())
                        .WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR,
                          mServiceOrientation.Initialize());

                EXPECT_CALL(mMockMpu9150, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockBarometer, Initialize())
                        .WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK,
                          mServiceOrientation.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_ORIENTATION() = default;

            /* Mocks */
            StrictMock<Component::Imu::MockMpu9150>         mMockMpu9150;
            StrictMock<Component::Barometer::MockBarometer> mMockBarometer;
            StrictMock<Event::MockEventDispatcherInterface>
                    mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface> mMockMessageInterface;

            /* Test class */
            ServiceOrientation mServiceOrientation;
        };

        TEST_F(UT_SRV_ORIENTATION, Update_Ok) {
            EXPECT_CALL(mMockMpu9150, Update(_)).Times(1U);
            EXPECT_CALL(mMockBarometer, Update(_)).Times(1U);

            mServiceOrientation.Update(0UL);
        }
    } // namespace Orientation
} // namespace Service
