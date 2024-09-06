#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/bot/MockBody.h"
#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/drv/MockUart.h"
#include "../../../mock/clu/MockClusters.h"


#include "../../../../src/Misc/Maths/Geometry.h"
#include "../../../../src/Cluster/Decoding/Protocol.h"
#include "../../../../src/Cluster/Body/ClusterBody.h"
#include "../../../../src/Component/Communication/Communication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Matcher;

namespace Component {
namespace Communication {
class UT_CMP_COMMUNICATION : public ::testing::Test {
protected:
	UT_CMP_COMMUNICATION() :
		mMockBody(),
		mMockUart(),
		mMockClusters(),
		mMockLed(),
		mClusterBody( mMockBody ),
		mCommunication( mMockUart, mMockClusters, mMockLed )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_COMMUNICATION() = default;

	/* Mocks */
	StrictMock <Bot::Body::MockBody> mMockBody;
	StrictMock <Driver::Uart::MockUart> mMockUart;
	StrictMock <ClusterBase::Clusters::MockClusters> mMockClusters;
	StrictMock <Component::Led::MockLed> mMockLed;
	ClusterBase::Body::ClusterBody mClusterBody;

	/* Test class */
	Communication mCommunication;
};

TEST_F( UT_CMP_COMMUNICATION, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockLed, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mCommunication.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_COMMUNICATION, Update_Ok_Noframe )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockLed, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockUart, DataAvailable() ).WillOnce( Return( 0U ) );

	success = mCommunication.Initialize();
	mCommunication.Update( 0UL );

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_COMMUNICATION, Update_Ok_1frame )
{
	Core::CoreStatus success  = Core::CoreStatus::CORE_ERROR;
	const char *     bufferRx = "<000000>";

	EXPECT_CALL( mMockLed, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mCommunication.Initialize();

	EXPECT_CALL( mMockLed, On() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Off() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockUart, Send( Matcher <const uint8_t *>( _ ), _ ) ).Times( 1U );
	EXPECT_CALL( mMockClusters, GetCluster( GENERAL ) ).Times( 1U );

	for ( size_t i = 0; i < strlen( bufferRx ); i++ )
	{
		EXPECT_CALL( mMockUart, DataAvailable() ).WillOnce( Return( 8U ) );
		EXPECT_CALL( mMockUart, Read() ).WillOnce( Return( bufferRx[i] ) );
		mCommunication.Update( 0UL );
	}

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_COMMUNICATION, Update_Ok_1frame_IMU )
{
	Core::CoreStatus success  = Core::CoreStatus::CORE_ERROR;
	const char *     bufferRx = "<010000>";

	EXPECT_CALL( mMockLed, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mCommunication.Initialize();

	EXPECT_CALL( mMockLed, On() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Off() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockUart, Send( Matcher <const uint8_t *>( _ ), _ ) ).Times( 1U );
	EXPECT_CALL( mMockClusters, GetCluster( IMU ) ).Times( 1U );

	for ( size_t i = 0; i < strlen( bufferRx ); i++ )
	{
		EXPECT_CALL( mMockUart, DataAvailable() ).WillOnce( Return( 8U ) );
		EXPECT_CALL( mMockUart, Read() ).WillOnce( Return( bufferRx[i] ) );
		mCommunication.Update( 0UL );
	}

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_COMMUNICATION, Update_Ko_1frame )
{
	Core::CoreStatus success  = Core::CoreStatus::CORE_ERROR;
	const char *     bufferRx = "<0<0000>";

	EXPECT_CALL( mMockLed, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mCommunication.Initialize();

	EXPECT_CALL( mMockLed, On() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Off() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockUart, Send( Matcher <const uint8_t *>( _ ), _ ) ).Times( 1U );

	for ( size_t i = 0; i < strlen( bufferRx ); i++ )
	{
		EXPECT_CALL( mMockUart, DataAvailable() ).WillOnce( Return( 8U ) );
		EXPECT_CALL( mMockUart, Read() ).WillOnce( Return( bufferRx[i] ) );
		mCommunication.Update( 0UL );
	}

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_COMMUNICATION, Update_Ok_BodySet )
{
	Core::CoreStatus           success  = Core::CoreStatus::CORE_ERROR;
	Misc::Maths::Position3d position = { -5.0, 5.0, -5.0 };
	Misc::Maths::Rotation3d rotation = { -3.0, 3.0, -2.0 };
	uint16_t travelTime = 500U;

	const char *bufferRx = "<05000ECEFF3200CEFFE2FF1E00ECFFF401>";

	EXPECT_CALL( mMockLed, Initialize() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mCommunication.Initialize();

	EXPECT_CALL( mMockLed, On() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockLed, Off() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockUart, Send( Matcher <const uint8_t *>( _ ), _ ) ).Times( 1U );
	EXPECT_CALL( mMockClusters, GetCluster( ClusterBase::EClusters::BODY ) ).WillOnce( Return( &mClusterBody ) );
	EXPECT_CALL( mMockBody, SetPositionRotation( position, rotation, travelTime ) ).Times( 1U );

	for ( size_t i = 0; i < strlen( bufferRx ); i++ )
	{
		EXPECT_CALL( mMockUart, DataAvailable() ).WillOnce( Return( 8U ) );
		EXPECT_CALL( mMockUart, Read() ).WillOnce( Return( bufferRx[i] ) );
		mCommunication.Update( 0UL );
	}

	EXPECT_TRUE( success );
}
}
}
