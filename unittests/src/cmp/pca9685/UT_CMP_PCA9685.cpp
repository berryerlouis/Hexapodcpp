#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTwi.h"

#include "../../../../src/cmp/ProximityInterface.h"
#include "../../../../src/cmp/Pca9685.h"

using ::testing::_;
using ::testing::StrictMock;
using ::testing::Return;

using namespace Component;

TEST( ComponentPca9685, Initialize_Ok )
{
	Core::CoreStatus     success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTwi> twi;

	Pca9685 pca9685( twi );

	EXPECT_CALL( twi, WriteRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( twi, ReadRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( twi, WriteRegister( _, (uint8_t) Pca9685::ERegister::PRESCALE, _ ) ).WillOnce( Return( true ) );

	success = pca9685.Initialize();

	EXPECT_TRUE( success );
}

TEST( ComponentPca9685, Update_Ok )
{
	Core::CoreStatus     success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTwi> twi;

	Pca9685 pca9685( twi );

	EXPECT_CALL( twi, WriteRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( twi, ReadRegister( _, Pca9685::ERegister::MODE1, _ ) ).WillRepeatedly( Return( true ) );
	EXPECT_CALL( twi, WriteRegister( _, (uint8_t) Pca9685::ERegister::PRESCALE, _ ) ).WillOnce( Return( true ) );


	EXPECT_CALL( twi, WriteRegisters( _, (uint8_t) Pca9685::ERegister::LED0_ON_L, _, _ ) ).WillOnce( Return( true ) );

	success = pca9685.Initialize();
	pca9685.Update( 0U );

	EXPECT_TRUE( success );
}
