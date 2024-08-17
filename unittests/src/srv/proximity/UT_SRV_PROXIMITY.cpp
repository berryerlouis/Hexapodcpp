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

class UT_SRV_PROXIMITY : public ::testing::Test  {
protected:
	UT_SRV_PROXIMITY() :
		mMockSensorProximity(),
		mMockServiceMediator(),
		mClusterProximity( mMockSensorProximity ),
		mServiceProximity( mClusterProximity, mMockSensorProximity )
	{
	}

	virtual void SetUp ()
	{
		mServiceProximity.SetComComponent( &mMockServiceMediator );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_PROXIMITY() = default;

	/* Mocks */
	StrictMock <MockSensorProximity> mMockSensorProximity;
	StrictMock <MockServiceMediator> mMockServiceMediator;

	ClusterProximity mClusterProximity;

	/* Test class */
	ServiceProximity mServiceProximity;
};

TEST_F( UT_SRV_PROXIMITY, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceProximity.Initialize();

	EXPECT_TRUE( success );
}


TEST_F( UT_SRV_PROXIMITY, Update_NoDetection_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceProximity.Initialize();

	EXPECT_CALL( mMockSensorProximity, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( false ) );

	mServiceProximity.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_PROXIMITY, Update_Detection_Srf_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceProximity.Initialize();

	EXPECT_CALL( mMockSensorProximity, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( true ) );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( false ) );
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( 10U ) );

	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServiceMediator, DisplayProximitySensor( SensorProximityInterface::SensorsId::SRF_LEFT ) ).Times( 1U );

	mServiceProximity.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST_F( UT_SRV_PROXIMITY, Update_Detection_Vlx_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mServiceProximity.Initialize();

	EXPECT_CALL( mMockSensorProximity, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_LEFT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::SRF_RIGHT ) ).WillOnce( Return( false ) );
	EXPECT_CALL( mMockSensorProximity, IsDetecting( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( true ) );
	EXPECT_CALL( mMockSensorProximity, GetDistance( SensorProximityInterface::SensorsId::VLX ) ).WillOnce( Return( 10U ) );

	EXPECT_CALL( mMockServiceMediator, SendFrame( _ ) ).Times( 1U );
	EXPECT_CALL( mMockServiceMediator, DisplayProximitySensor( SensorProximityInterface::SensorsId::VLX ) ).Times( 1U );

	mServiceProximity.Update( 0UL );

	EXPECT_TRUE( success );
}
