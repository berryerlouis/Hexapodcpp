#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/cmp/MockBarometer.h"
#include "../../../mock/cmp/MockMpu9150.h"
#include "../../../mock/srv/MockEventListener.h"

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
			UT_SRV_ORIENTATION() : mMockMpu9150(),
			                       mMockBarometer(),
			                       mMockEventListener(),
			                       mServiceOrientation(mMockMpu9150, mMockBarometer, mMockEventListener) {
			}

			virtual void SetUp() {
				EXPECT_CALL(mMockMpu9150, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_ERROR));
				EXPECT_EQ(Core::CoreStatus::CORE_ERROR, mServiceOrientation.Initialize());

				EXPECT_CALL(mMockMpu9150, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_CALL(mMockBarometer, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_EQ(Core::CoreStatus::CORE_OK, mServiceOrientation.Initialize());
			}

			virtual void TearDown() {
			}

			virtual ~UT_SRV_ORIENTATION() = default;

			/* Mocks */
			StrictMock<Component::Imu::MockMpu9150> mMockMpu9150;
			StrictMock<Component::Barometer::MockBarometer> mMockBarometer;
			StrictMock<Event::MockEventListener> mMockEventListener;

			/* Test class */
			ServiceOrientation mServiceOrientation;
		};

		TEST_F(UT_SRV_ORIENTATION, Update_Ok) {
			EXPECT_CALL(mMockMpu9150, Update( _ )).Times(1U);
			EXPECT_CALL(mMockBarometer, Update( _ )).Times(1U);

			mServiceOrientation.Update(0UL);
		}
	}
}
