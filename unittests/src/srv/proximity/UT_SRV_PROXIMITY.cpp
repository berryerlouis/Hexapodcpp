#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/srv/MockServiceMediator.h"
#include "../../../mock/cmp/MockSrf05.h"
#include "../../../mock/cmp/MockVl53l0x.h"

#include "../../../../src/Cluster/Proximity/ClusterProximity.h"
#include "../../../../src/Service/Proximity/ServiceProximity.h"
#include "../../../../src/Component/Proximity/SensorProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service {
namespace Proximity {
class UT_SRV_PROXIMITY : public ::testing::Test {
protected:
	UT_SRV_PROXIMITY() :
		mMockSrf05Left(),
		mMockSrf05Right(),
		mMockVl53l0x(),
		mSensorProximity( mMockSrf05Left, mMockSrf05Right, mMockVl53l0x ),
		mClusterProximity( mSensorProximity ),
		mMockServiceMediator(),
		mServiceProximity( mClusterProximity, mSensorProximity )
	{
	}

	virtual void SetUp ()
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		EXPECT_CALL( mMockSrf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_CALL( mMockSrf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_CALL( mMockVl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

		success = mServiceProximity.Initialize();
		mServiceProximity.SetEventManager( &mMockServiceMediator );

		EXPECT_TRUE( success );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_PROXIMITY() = default;

	/* Class */
	Component::Proximity::SensorProximity mSensorProximity;
	Cluster::Proximity::ClusterProximity mClusterProximity;

	/* Mocks */
	StrictMock <Component::Proximity::Ultrasound::MockSrf05> mMockSrf05Left;
	StrictMock <Component::Proximity::Ultrasound::MockSrf05> mMockSrf05Right;
	StrictMock <Component::Proximity::Laser::MockVl53l0x> mMockVl53l0x;
	StrictMock <MockServiceMediator> mMockServiceMediator;

	/* Test class */
	ServiceProximity mServiceProximity;
};

TEST_F( UT_SRV_PROXIMITY, Update_NoDetection_Ok )
{
	EXPECT_CALL( mMockSrf05Left, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockSrf05Right, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockVl53l0x, Update( _ ) ).Times( 1U );
	mServiceProximity.Update( 0UL );
}

TEST_F( UT_SRV_PROXIMITY, Update_Detection_Srf_Ok )
{
	EXPECT_CALL( mMockSrf05Left, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockSrf05Right, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockVl53l0x, Update( _ ) ).Times( 1U );
	mServiceProximity.Update( 0UL );
}
}
}
