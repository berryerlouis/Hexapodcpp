#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSoftware.h"
#include "../../../mock/cmp/MockBattery.h"
#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../mock/cmp/MockCommunication.h"
#include "../../../mock/clu/MockClusters.h"
#include "../../../mock/cor/MockEventMediatorInterface.h"

#include "../../../../src/Cluster/Battery/ClusterBattery.h"
#include "../../../../src/Cluster/General/ClusterGeneral.h"
#include "../../../../src/Cluster/Proximity/ClusterProximity.h"
#include "../../../../src/Service/Communication/ServiceCommunication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace Communication {
class UT_SRV_COMMUNICATION : public ::testing::Test {
protected:
	UT_SRV_COMMUNICATION() :
		mMockBattery(),
		mMockSoftware(),
		mMockSensorProximity(),
		mMockCommunication(),
		mClusterBattery( mMockBattery ),
		mClusterProximity( mMockSensorProximity ),
		mClusterGeneral( mMockSoftware ),
		mMockClusters(),
		mMockEventMediatorInterface(),
		mServiceCommunication( mMockCommunication, mMockClusters )
	{
	}

	virtual void SetUp ()
	{
		EXPECT_CALL( mMockCommunication, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );
		EXPECT_FALSE( mServiceCommunication.Initialize() );

		EXPECT_CALL( mMockCommunication, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_TRUE( mServiceCommunication.Initialize() );
		mServiceCommunication.setMediator( &mMockEventMediatorInterface );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_COMMUNICATION() = default;


	/* Mocks */
	StrictMock <Component::Software::MockSoftware> mMockSoftware;
	StrictMock <Component::Battery::MockBattery> mMockBattery;
	StrictMock <Component::Proximity::MockSensorProximity> mMockSensorProximity;
	StrictMock <Component::Communication::MockCommunication> mMockCommunication;
	Cluster::Battery::ClusterBattery mClusterBattery;
	Cluster::Proximity::ClusterProximity mClusterProximity;
	Cluster::General::ClusterGeneral mClusterGeneral;
	StrictMock <Cluster::Clusters::MockClusters> mMockClusters;
	StrictMock <Core::MockEventMediatorInterface> mMockEventMediatorInterface;

	/* Test class */
	ServiceCommunication mServiceCommunication;
};

TEST_F( UT_SRV_COMMUNICATION, Initialize_Update )
{
	EXPECT_CALL( mMockCommunication, Update( 12450UL ) ).Times( 1U );

	mServiceCommunication.Update( 12450UL );
}

TEST_F( UT_SRV_COMMUNICATION, Initialize_SendMessageGeneral_Ok )
{
	Core::Event event = { id:Cluster::EClusters::GENERAL, value: Cluster::EGeneralCommands::MIN_EXECUTION_TIME };
	EXPECT_CALL( mMockClusters, GetCluster( Cluster::EClusters::GENERAL ) ).WillOnce( Return( &mClusterGeneral ) );
	EXPECT_CALL( mMockSoftware, GetMinTime() ).Times( 1U ).WillOnce( Return( 0UL ) );
	EXPECT_CALL( mMockCommunication, SendMessage( _ ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	mServiceCommunication.SendMessage( event );
}

TEST_F( UT_SRV_COMMUNICATION, Initialize_SendMessageBattery_Ok )
{
	Core::Event event = { id:Cluster::EClusters::BATTERY, value: Cluster::EBatteryCommands::GET_BAT_STATUS };
	EXPECT_CALL( mMockClusters, GetCluster( Cluster::EClusters::BATTERY ) ).WillOnce( Return( &mClusterBattery ) );
	EXPECT_CALL( mMockBattery, GetVoltage() ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockBattery, GetState() ).WillOnce( Return( Component::Battery::BatteryState::CRITICAL ) );
	EXPECT_CALL( mMockCommunication, SendMessage( _ ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	mServiceCommunication.SendMessage( event );
}

TEST_F( UT_SRV_COMMUNICATION, Initialize_SendMessageProximity_Ok )
{
	Core::Event event = { id:Cluster::EClusters::PROXIMITY, value: Cluster::EProximityCommands::LASER };
	EXPECT_CALL( mMockClusters, GetCluster( Cluster::EClusters::PROXIMITY ) ).WillOnce( Return( &mClusterProximity ) );
	EXPECT_CALL( mMockSensorProximity, GetDistance( Component::Proximity::SensorsId::VLX ) ).WillOnce( Return( 10U ) );
	EXPECT_CALL( mMockCommunication, SendMessage( _ ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	mServiceCommunication.SendMessage( event );
}
}
}
