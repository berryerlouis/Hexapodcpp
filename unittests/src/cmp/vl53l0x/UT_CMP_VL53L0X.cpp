#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTwi.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/Component/Proximity/SensorProximityInterface.h"
#include "../../../../src/Component/Proximity/Laser/Vl53l0x.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::SetArgReferee;
using ::testing::DoAll;


namespace Component {
namespace Proximity {
namespace Laser {
class UT_CMP_VL53L0X : public ::testing::Test {
protected:
	UT_CMP_VL53L0X() :
		mMockTick(),
		mMockTwi(),
		mVl53l0x( mMockTwi, mMockTick )
	{
	}

	virtual void SetUp ()
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		EXPECT_CALL( mMockTick, GetMs() ).WillRepeatedly( Return( 0U ) );
		EXPECT_CALL( mMockTwi, ReadRegister( _, _, _ ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mMockTwi, WriteRegister( _, _, _ ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mMockTwi, ReadRegisters( _, _, _, _ ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mMockTwi, WriteRegisters( _, _, _, _ ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mMockTwi, ReadRegister16Bits( _, _, _ ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mMockTwi, WriteRegister16Bits( _, _, _ ) ).WillRepeatedly( Return( true ) );
		EXPECT_CALL( mMockTwi, WriteRegister32Bits( _, _, _ ) ).WillRepeatedly( Return( true ) );

		EXPECT_CALL( mMockTwi, ReadRegister( _, VL53L0X_IDENTIFICATION_MODEL_ID, _ ) ).WillOnce( DoAll( SetArgReferee <2U>( 0xEEU ), Return( true ) ) );
		EXPECT_CALL( mMockTwi, ReadRegister( _, 0x83, _ ) ).WillRepeatedly( DoAll( SetArgReferee <2U>( 1U ), Return( true ) ) );
		EXPECT_CALL( mMockTwi, ReadRegister( _, VL53L0X_RESULT_INTERRUPT_STATUS, _ ) ).WillRepeatedly( DoAll( SetArgReferee <2U>( 0x07U ), Return( true ) ) );

		success = mVl53l0x.Initialize();

		EXPECT_TRUE( success );
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_VL53L0X() = default;

	/* Mocks */
	StrictMock <Driver::Tick::MockTick> mMockTick;
	StrictMock <Driver::Twi::MockTwi> mMockTwi;

	/* Test class */
	Vl53l0x mVl53l0x;
};

TEST_F( UT_CMP_VL53L0X, Update_Ok )
{
	mVl53l0x.Update( 0U );
}

TEST_F( UT_CMP_VL53L0X, SetThreshold )
{
	EXPECT_TRUE( mVl53l0x.SetThreshold( 350U ) );
	EXPECT_EQ( 350U, mVl53l0x.GetThreshold() );
}

TEST_F( UT_CMP_VL53L0X, UnderThreshold )
{
	EXPECT_CALL( mMockTwi, ReadRegister( _, VL53L0X_RESULT_INTERRUPT_STATUS, _ ) ).WillOnce( DoAll( SetArgReferee <2U>( 0x07U ), Return( true ) ) );
	EXPECT_CALL( mMockTwi, ReadRegister16Bits( _, VL53L0X_RESULT_RANGE_STATUS + 10, _ ) ).WillOnce( DoAll( SetArgReferee <2U>( 250U ), Return( true ) ) );

	mVl53l0x.Update( 0U );
}

TEST_F( UT_CMP_VL53L0X, AboveThreshold )
{
	EXPECT_CALL( mMockTwi, ReadRegister( _, VL53L0X_RESULT_INTERRUPT_STATUS, _ ) ).WillRepeatedly( DoAll( SetArgReferee <2U>( 0x07U ), Return( true ) ) );
	EXPECT_CALL( mMockTwi, ReadRegister16Bits( _, VL53L0X_RESULT_RANGE_STATUS + 10, _ ) ).WillOnce( DoAll( SetArgReferee <2U>( 350U ), Return( true ) ) );

	mVl53l0x.Update( 0U );
}
}
}
}
