#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/Component/ServosController/Pca9685.h"

using ::testing::_;
using ::testing::StrictMock;
using ::testing::Return;


namespace Component {
namespace ServosController {
class UT_CMP_PCA9685 : public ::testing::Test {
protected:
	UT_CMP_PCA9685() :
		mMockTwi(),
		mPca9685( mMockTwi )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_PCA9685() = default;

	/* Mocks */
	StrictMock <Driver::Twi::MockTwi> mMockTwi;

	/* Test class */
	Pca9685 mPca9685;
};

TEST_F( UT_CMP_PCA9685, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockTwi, WriteRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mMockTwi, ReadRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mMockTwi, WriteRegister( _, (uint8_t) Pca9685::ERegister::PRESCALE, _ ) ).WillOnce( Return( true ) );

	success = mPca9685.Initialize();

	EXPECT_EQ( success, Core::CoreStatus::CORE_OK );
}

TEST_F( UT_CMP_PCA9685, Update_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockTwi, WriteRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mMockTwi, ReadRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( mMockTwi, WriteRegister( _, (uint8_t) Pca9685::ERegister::PRESCALE, _ ) ).WillOnce( Return( true ) );


	EXPECT_CALL( mMockTwi, WriteRegisters( _, (uint8_t) Pca9685::ERegister::LED0_ON_L, _, _ ) ).WillOnce( Return( true ) );

	success = mPca9685.Initialize();
	mPca9685.Update( 0U );

	EXPECT_EQ( success, Core::CoreStatus::CORE_OK );
}
}
}
