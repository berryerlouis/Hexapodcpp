#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/srv/MockService.h"
#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../mock/cor/MockEventMediatorInterface.h"

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
		mMockSensorProximity(),
		mMockEventMediatorInterface(),
		mServiceProximity( mMockSensorProximity )
	{
	}

	virtual void SetUp ()
	{
		EXPECT_CALL( mMockSensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_ERROR ) );
		EXPECT_FALSE( mServiceProximity.Initialize() );

		EXPECT_CALL( mMockSensorProximity, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_CALL( mMockSensorProximity, Attach( _ ) ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
		EXPECT_TRUE( mServiceProximity.Initialize() );
		mServiceProximity.setMediator( &mMockEventMediatorInterface );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_SRV_PROXIMITY() = default;

	/* Mocks */
	StrictMock <Component::Proximity::MockSensorProximity> mMockSensorProximity;
	StrictMock <Core::MockEventMediatorInterface> mMockEventMediatorInterface;

	/* Test class */
	ServiceProximity mServiceProximity;
};

TEST_F( UT_SRV_PROXIMITY, Update_NoDetection_Ok )
{
	EXPECT_CALL( mMockSensorProximity, Update( _ ) ).Times( 1U );
	mServiceProximity.Update( 0UL );
}

TEST_F( UT_SRV_PROXIMITY, Update_Detection_Srf_Ok )
{
	EXPECT_CALL( mMockSensorProximity, Update( _ ) ).Times( 1U );
	mServiceProximity.Update( 0UL );
}
}
}
