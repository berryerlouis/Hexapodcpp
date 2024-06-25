#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockSensorProximity.h"

#include "../../../../src/clu/ClusterProximity.h"
#include "../../../../src/srv/ServiceProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST( ServiceProximity, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );
	ServiceProximity serviceProximity( clusterProximity );

	EXPECT_CALL( sensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = serviceProximity.Initialize();

	EXPECT_TRUE( success );
}


TEST( ServiceProximity, Update_NoDetection_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );
	ServiceProximity serviceProximity( clusterProximity );

	EXPECT_CALL( sensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = serviceProximity.Initialize();

	EXPECT_CALL( sensorProximity, Update( _ ) ).Times( 1U );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( false ) );

	serviceProximity.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST( ServiceProximity, Update_Detection_Srf_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockServiceMediator> mediator;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );
	ServiceProximity serviceProximity( clusterProximity );

	serviceProximity.SetComComponent( &mediator );

	EXPECT_CALL( sensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = serviceProximity.Initialize();

	EXPECT_CALL( sensorProximity, Update( _ ) ).Times( 1U );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( true ) );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( false ) );
	EXPECT_CALL( sensorProximity, GetDistance( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( 10U ) );

	EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mediator, DisplayProximitySensor( SensorProximityInterface::SensorsId::SRF_LEFT ) ).Times( 1U );

	serviceProximity.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST( ServiceProximity, Update_Detection_Vlx_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockServiceMediator> mediator;
	StrictMock <MockSensorProximity> sensorProximity;
	ClusterProximity clusterProximity( sensorProximity );
	ServiceProximity serviceProximity( clusterProximity );

	serviceProximity.SetComComponent( &mediator );

	EXPECT_CALL( sensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = serviceProximity.Initialize();

	EXPECT_CALL( sensorProximity, Update( _ ) ).Times( 1U );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( sensorProximity, IsDetecting( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( true ) );
	EXPECT_CALL( sensorProximity, GetDistance( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( 10U ) );

	EXPECT_CALL( mediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mediator, DisplayProximitySensor( SensorProximityInterface::SensorsId::VLX ) ).Times( 1U );

	serviceProximity.Update( 0UL );

	EXPECT_TRUE( success );
}
