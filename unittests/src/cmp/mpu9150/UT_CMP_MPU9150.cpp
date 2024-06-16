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

TEST(ComponentMpu9150, Initialize_Ok)
{
	bool success = false;
	StrictMock <MockTwi> twi;

	Mpu9150 mpu9150(twi);

	EXPECT_CALL(twi, ReadRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister(_, _, _) ).WillRepeatedly(Return(true) );

	EXPECT_CALL(twi, ReadRegister(Mpu9150::MPU9150_I2C_ADDRESS, Mpu9150::ERegister::WHO_AM_I, _) ).WillOnce(DoAll(SetArgReferee <2U>(Mpu9150::MPU9150_I2C_ADDRESS - 1), Return(true) ) );

	success = mpu9150.Initialize();

	EXPECT_TRUE(success);
}

TEST(ComponentMpu9150, Update_Ok)
{
	bool success = false;
	StrictMock <MockTwi> twi;

	Mpu9150 mpu9150(twi);

	EXPECT_CALL(twi, ReadRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister(_, _, _) ).WillRepeatedly(Return(true) );

	EXPECT_CALL(twi, ReadRegister(Mpu9150::MPU9150_I2C_ADDRESS, Mpu9150::ERegister::WHO_AM_I, _) ).WillOnce(DoAll(SetArgReferee <2U>(Mpu9150::MPU9150_I2C_ADDRESS - 1), Return(true) ) );

	success = mpu9150.Initialize();
	mpu9150.Update(0UL);

	EXPECT_TRUE(success);
}
