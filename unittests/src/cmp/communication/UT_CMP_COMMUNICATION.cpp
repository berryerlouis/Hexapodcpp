#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/cmp/MockLed.h"
#include "../../../mock/drv/MockUart.h"
#include "../../../mock/clu/MockClusters.h"


#include "../../../../src/clu/Protocol.h"
#include "../../../../src/cmp/Communication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::Matcher;

using namespace Component;

class UT_CMP_COMMUNICATION : public ::testing::Test {
protected:
	UT_CMP_COMMUNICATION() :
		mMockUart(),
		mMockClusters(),
		mMockLed(),
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
	StrictMock <MockUart> mMockUart;
	StrictMock <MockClusters> mMockClusters;
	StrictMock <MockLed> mMockLed;

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
	EXPECT_CALL( mMockClusters, GetCluster( Clusters::EClusters::GENERAL ) ).Times( 1U );

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
	EXPECT_CALL( mMockClusters, GetCluster( Clusters::EClusters::IMU ) ).Times( 1U );

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
