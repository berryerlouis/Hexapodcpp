#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSrf05.h"
#include "../../../mock/cmp/MockVl53l0x.h"

#include "../../../../src/cmp/SensorProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;

TEST( SensorProximity, Initialize_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity( srf05Left, srf05Right, vl53l0x );

	EXPECT_CALL( srf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( srf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( vl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = sensorProximity.Initialize();

	EXPECT_TRUE( success );
}

TEST( SensorProximity, Update_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity( srf05Left, srf05Right, vl53l0x );

	EXPECT_CALL( srf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( srf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( vl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	EXPECT_CALL( srf05Left, Update( _ ) ).Times( 1U );
	EXPECT_CALL( srf05Right, Update( _ ) ).Times( 0U );
	EXPECT_CALL( vl53l0x, Update( _ ) ).Times( 1U );

	success = sensorProximity.Initialize();

	sensorProximity.Update( 0UL );
	EXPECT_TRUE( success );
}

TEST( SensorProximity, Update2Times_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity( srf05Left, srf05Right, vl53l0x );

	EXPECT_CALL( srf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( srf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( vl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	EXPECT_CALL( srf05Left, Update( _ ) ).Times( 1U );
	EXPECT_CALL( srf05Right, Update( _ ) ).Times( 1U );
	EXPECT_CALL( vl53l0x, Update( _ ) ).Times( 2U );

	success = sensorProximity.Initialize();

	sensorProximity.Update( 0UL );
	sensorProximity.Update( 0UL );
	EXPECT_TRUE( success );
}

TEST( SensorProximity, GetDistance_Ok )
{
	Core::CoreStatus         success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity( srf05Left, srf05Right, vl53l0x );

	EXPECT_CALL( srf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( srf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( vl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	EXPECT_CALL( srf05Left, GetDistance() ).Times( 1U );
	EXPECT_CALL( srf05Right, GetDistance() ).Times( 1U );
	EXPECT_CALL( vl53l0x, GetDistance() ).Times( 1U );

	success = sensorProximity.Initialize();

	for ( size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++ )
	{
		sensorProximity.GetDistance( (SensorProximity::SensorsId) sensorId );
	}
	EXPECT_TRUE( success );
}

TEST( SensorProximity, SetThreshold_Ok )
{
	Core::CoreStatus         success   = Core::CoreStatus::CORE_ERROR;
	const uint16_t           threshold = 10U;
	StrictMock <MockSrf05>   srf05Left;
	StrictMock <MockSrf05>   srf05Right;
	StrictMock <MockVl53l0x> vl53l0x;

	SensorProximity sensorProximity( srf05Left, srf05Right, vl53l0x );

	EXPECT_CALL( srf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( srf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( vl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = sensorProximity.Initialize();

	EXPECT_CALL( srf05Left, SetThreshold( threshold ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( srf05Right, SetThreshold( threshold ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( vl53l0x, SetThreshold( threshold ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );


	for ( size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++ )
	{
		EXPECT_TRUE( sensorProximity.SetThreshold( (SensorProximityInterface::SensorsId) sensorId, threshold ) );
	}
	EXPECT_TRUE( success );
}
