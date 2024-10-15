#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../mock/srv/MockEventListener.h"

#include "../../../../src/Service/Proximity/ServiceProximity.h"
#include "../../../../src/Component/Proximity/SensorProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
	namespace Proximity
	{
		class UT_SRV_PROXIMITY : public ::testing::Test {
		protected:
			UT_SRV_PROXIMITY() : mMockSensorProximity(),
			                     mMockEventListener(),
			                     mServiceProximity(mMockSensorProximity, mMockEventListener) {
			}

			virtual void SetUp() {
				EXPECT_CALL(mMockSensorProximity, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_ERROR));
				EXPECT_CALL(mMockSensorProximity, Attach( _ )).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_EQ(Core::CoreStatus::CORE_OK, mServiceProximity.Initialize());

				EXPECT_CALL(mMockSensorProximity, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_CALL(mMockSensorProximity, Attach( _ )).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_EQ(Core::CoreStatus::CORE_OK, mServiceProximity.Initialize());
			}

			virtual void TearDown() {
			}

			virtual ~UT_SRV_PROXIMITY() = default;

			/* Mocks */
			StrictMock<Component::Proximity::MockSensorProximity> mMockSensorProximity;
			StrictMock<Event::MockEventListener> mMockEventListener;

			/* Test class */
			ServiceProximity mServiceProximity;
		};

		TEST_F(UT_SRV_PROXIMITY, Update) {
			EXPECT_CALL(mMockSensorProximity, Update( 12450UL )).Times(1U);
			mServiceProximity.Update(12450UL);
		}

		TEST_F(UT_SRV_PROXIMITY, Detect) {
			constexpr SensorsId sensorId = SensorsId::SRF_LEFT;
			constexpr uint16_t distance = 42U;
			constexpr uint8_t arg[2U] = {
				static_cast<uint8_t>(distance >> 8U),
				static_cast<uint8_t>(distance & 0xFFU)
			};
			const SEvent ev(PROXIMITY, sensorId, arg, 2U);

			EXPECT_CALL(mMockEventListener, AddEvent(ev)).Times(1U);
			mServiceProximity.Detect(sensorId, distance);
		}
	}
}
