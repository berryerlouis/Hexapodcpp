#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSoftware.h"
#include "../../../mock/cmp/MockBattery.h"
#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../mock/cmp/MockCommunication.h"
#include "../../../mock/clu/MockClusters.h"
#include "../../../mock/srv/MockEventListener.h"

#include "../../../../src/Cluster/Battery/ClusterBattery.h"
#include "../../../../src/Cluster/General/ClusterGeneral.h"
#include "../../../../src/Cluster/Proximity/ClusterProximity.h"
#include "../../../../src/Service/Communication/ServiceCommunication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
	using namespace Battery;

	namespace Communication
	{
		class UT_SRV_COMMUNICATION : public ::testing::Test {
		protected:
			UT_SRV_COMMUNICATION() : mMockBattery(),
			                         mMockSoftware(),
			                         mMockSensorProximity(),
			                         mMockCommunication(),
			                         mClusterBattery(mMockBattery),
			                         mClusterProximity(mMockSensorProximity),
			                         mClusterGeneral(mMockSoftware),
			                         mMockClusters(),
			                         mMockEventListener(),
			                         mServiceCommunication(mMockCommunication, mMockClusters, mMockEventListener) {
			}

			virtual void SetUp() {
				EXPECT_CALL(mMockCommunication, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_ERROR));
				EXPECT_EQ(Core::CoreStatus::CORE_ERROR, mServiceCommunication.Initialize());

				EXPECT_CALL(mMockCommunication, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_EQ(Core::CoreStatus::CORE_OK, mServiceCommunication.Initialize());
			}

			virtual void TearDown() {
			}

			virtual ~UT_SRV_COMMUNICATION() = default;


			/* Mocks */
			StrictMock<Component::Software::MockSoftware> mMockSoftware;
			StrictMock<Component::Battery::MockBattery> mMockBattery;
			StrictMock<Component::Proximity::MockSensorProximity> mMockSensorProximity;
			StrictMock<Component::Communication::MockCommunication> mMockCommunication;
			Cluster::Battery::ClusterBattery mClusterBattery;
			Cluster::Proximity::ClusterProximity mClusterProximity;
			Cluster::General::ClusterGeneral mClusterGeneral;
			StrictMock<Cluster::Clusters::MockClusters> mMockClusters;
			StrictMock<Event::MockEventListener> mMockEventListener;

			/* Test class */
			ServiceCommunication mServiceCommunication;
		};

		TEST_F(UT_SRV_COMMUNICATION, Initialize_Update) {
			EXPECT_CALL(mMockCommunication, Update( 12450UL )).Times(1U);
			mServiceCommunication.Update(12450UL);
		}

		TEST_F(UT_SRV_COMMUNICATION, DispatchEvent_BatteryState) {
			constexpr BatteryState batteryState = BatteryState::WARNING;
			constexpr uint8_t voltage = 10U;
			constexpr uint8_t arg[1U] = {static_cast<uint8_t>(voltage)};
			const SEvent ev(BATTERY, batteryState, arg, 1U);

			EXPECT_CALL(mMockClusters, GetCluster( Cluster::EClusters::BATTERY )).
					WillOnce(Return(&mClusterBattery));
			EXPECT_CALL(mMockCommunication, SendMessage( _ )).Times(1U).WillOnce(Return(Core::CoreStatus::CORE_OK));
			mServiceCommunication.DispatchEvent(ev);
		}

		TEST_F(UT_SRV_COMMUNICATION, DispatchEvent_Distance) {
			constexpr uint16_t distance = 42U;
			constexpr uint8_t arg[2U] = {
				static_cast<uint8_t>(distance >> 8U),
				static_cast<uint8_t>(distance & 0xFFU)
			};
			const SEvent ev(EServices::PROXIMITY, Proximity::SensorsId::SRF_LEFT, arg, 2U);

			EXPECT_CALL(mMockClusters, GetCluster( Cluster::EClusters::PROXIMITY )).
					WillOnce(Return(&mClusterProximity));
			EXPECT_CALL(mMockCommunication, SendMessage( _ )).Times(1U).WillOnce(Return(Core::CoreStatus::CORE_OK));
			mServiceCommunication.DispatchEvent(ev);
		}

		TEST_F(UT_SRV_COMMUNICATION, DispatchEvent_MaxTime) {
			constexpr uint16_t distance = 42U;
			constexpr uint8_t arg[2U] = {
				static_cast<uint8_t>(distance >> 8U),
				static_cast<uint8_t>(distance & 0xFFU)
			};
			const SEvent ev(EServices::GENERAL, EGeneralCommands::MIN_EXECUTION_TIME, arg, 2U);

			EXPECT_CALL(mMockClusters, GetCluster( Cluster::EClusters::GENERAL )).
					WillOnce(Return(&mClusterProximity));
			EXPECT_CALL(mMockCommunication, SendMessage( _ )).Times(1U).WillOnce(Return(Core::CoreStatus::CORE_OK));
			mServiceCommunication.DispatchEvent(ev);
		}
	}
}
