#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/cmp/Mpu9150.h"

using ::testing::StrictMock;
using ::testing::_;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::Return;

using namespace Component;

class UT_CMP_MPU9150 : public ::testing::Test {
protected:
	UT_CMP_MPU9150() :
		mMockTwi(),
		mMpu9150( mMockTwi )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_MPU9150() = default;

	/* Mocks */
	StrictMock <MockTwi> mMockTwi;

	/* Test class */
	Mpu9150 mMpu9150;
};

TEST_F( UT_CMP_MPU9150, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockTwi, ReadRegister( _, _, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mMockTwi, WriteRegister( _, _, _ ) ).WillRepeatedly( Return( true ) );

	EXPECT_CALL( mMockTwi, ReadRegister( Mpu9150::MPU9150_I2C_ADDRESS, Mpu9150::ERegister::WHO_AM_I, _ ) ).WillOnce( DoAll( SetArgReferee <2U>( Mpu9150::MPU9150_I2C_ADDRESS - 1 ), Return( true ) ) );

	success = mMpu9150.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_MPU9150, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockTwi, ReadRegister( _, _, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mMockTwi, ReadRegisters( _, _, _, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mMockTwi, WriteRegister( _, _, _ ) ).WillRepeatedly( Return( true ) );

	EXPECT_CALL( mMockTwi, ReadRegister( Mpu9150::MPU9150_I2C_ADDRESS, Mpu9150::ERegister::WHO_AM_I, _ ) ).WillOnce( DoAll( SetArgReferee <2U>( Mpu9150::MPU9150_I2C_ADDRESS - 1 ), Return( true ) ) );

	success = mMpu9150.Initialize();
	mMpu9150.Update( 0UL );

	EXPECT_TRUE( success );
}
