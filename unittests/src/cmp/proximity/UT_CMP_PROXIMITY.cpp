#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockSrf05.h"
#include "../../../mock/cmp/MockVl53l0x.h"

#include "../../../../src/Component/Proximity/SensorProximity.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component {
namespace Proximity {
class UT_CMP_PROXIMITY : public ::testing::Test {
protected:
	UT_CMP_PROXIMITY() :
		mMockSrf05Left(),
		mMockSrf05Right(),
		mMockVl53l0x(),
		mSensorProximity( mMockSrf05Left, mMockSrf05Right, mMockVl53l0x )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_PROXIMITY() = default;

	/* Mocks */
	StrictMock <Component::Proximity::Ultrasound::MockSrf05> mMockSrf05Left;
	StrictMock <Component::Proximity::Ultrasound::MockSrf05> mMockSrf05Right;
	StrictMock <Component::Proximity::Laser::MockVl53l0x> mMockVl53l0x;

	/* Test class */
	SensorProximity mSensorProximity;
};

TEST_F( UT_CMP_PROXIMITY, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSrf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockSrf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockVl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mSensorProximity.Initialize();

	EXPECT_EQ( success, Core::CoreStatus::CORE_OK );
}

TEST_F( UT_CMP_PROXIMITY, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSrf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockSrf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockVl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	EXPECT_CALL( mMockSrf05Left, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockSrf05Right, Update( _ ) ).Times( 1U );
	EXPECT_CALL( mMockVl53l0x, Update( _ ) ).Times( 1U );

	success = mSensorProximity.Initialize();

	mSensorProximity.Update( 0UL );
	EXPECT_EQ( success, Core::CoreStatus::CORE_OK );
}

TEST_F( UT_CMP_PROXIMITY, Update2Times_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSrf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockSrf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockVl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	EXPECT_CALL( mMockSrf05Left, Update( _ ) ).Times( 2U );
	EXPECT_CALL( mMockSrf05Right, Update( _ ) ).Times( 2U );
	EXPECT_CALL( mMockVl53l0x, Update( _ ) ).Times( 2U );

	success = mSensorProximity.Initialize();

	mSensorProximity.Update( 0UL );
	mSensorProximity.Update( 0UL );
	EXPECT_EQ( success, Core::CoreStatus::CORE_OK );
}

TEST_F( UT_CMP_PROXIMITY, GetDistance_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockSrf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockSrf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockVl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	EXPECT_CALL( mMockSrf05Left, GetDistance() ).Times( 1U );
	EXPECT_CALL( mMockSrf05Right, GetDistance() ).Times( 1U );
	EXPECT_CALL( mMockVl53l0x, GetDistance() ).Times( 1U );

	success = mSensorProximity.Initialize();

	for ( size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++ )
	{
		mSensorProximity.GetDistance( (SensorsId) sensorId );
	}
	EXPECT_EQ( success, Core::CoreStatus::CORE_OK );
}

TEST_F( UT_CMP_PROXIMITY, SetThreshold_Ok )
{
	Core::CoreStatus success   = Core::CoreStatus::CORE_ERROR;
	const uint16_t   threshold = 10U;

	EXPECT_CALL( mMockSrf05Left, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockSrf05Right, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockVl53l0x, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	success = mSensorProximity.Initialize();

	EXPECT_CALL( mMockSrf05Left, SetThreshold( threshold ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockSrf05Right, SetThreshold( threshold ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockVl53l0x, SetThreshold( threshold ) ).Times( 1U ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );


	for ( size_t sensorId = 0U; sensorId < SensorProximity::NB_SENSORS; sensorId++ )
	{
		EXPECT_EQ(Core::CoreStatus::CORE_OK, mSensorProximity.SetThreshold( static_cast<SensorsId>(sensorId), threshold ));
	}
	EXPECT_EQ( success, Core::CoreStatus::CORE_OK );
}
}
}
