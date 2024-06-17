#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockTwi.h"
#include "../../../mock/drv/MockTick.h"

#include "../../../../src/cmp/ProximityInterface.h"
#include "../../../../src/cmp/Vl53l0x.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;
using ::testing::SetArgReferee;
using ::testing::DoAll;

using namespace Component;

TEST(ComponentVl53l0x, Initialize_Ok)
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	uint8_t               data;
	StrictMock <MockTick> tick;
	StrictMock <MockTwi>  twi;

	Vl53l0x vl53l0x(twi, tick);
	data = 0xEEU;
	EXPECT_CALL(tick, GetMs() ).WillRepeatedly(Return(0U) );
	EXPECT_CALL(twi, ReadRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );

	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_IDENTIFICATION_MODEL_ID, _) ).WillOnce(DoAll(SetArgReferee <2U>(0xEEU), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, 0x83, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(1U), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_RESULT_INTERRUPT_STATUS, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(0x07U), Return(true) ) );

	success = vl53l0x.Initialize();

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}

TEST(ComponentVl53l0x, Update_Ok)
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick> tick;
	StrictMock <MockTwi>  twi;

	Vl53l0x vl53l0x(twi, tick);

	EXPECT_CALL(tick, GetMs() ).WillRepeatedly(Return(0U) );
	EXPECT_CALL(twi, ReadRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );

	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_IDENTIFICATION_MODEL_ID, _) ).WillOnce(DoAll(SetArgReferee <2U>(0xEEU), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, 0x83, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(1U), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_RESULT_INTERRUPT_STATUS, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(0x07U), Return(true) ) );

	success = vl53l0x.Initialize();
	vl53l0x.Update(0U);

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}

TEST(ComponentVl53l0x, IsDetecting_SetThreshold)
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick> tick;
	StrictMock <MockTwi>  twi;

	Vl53l0x vl53l0x(twi, tick);

	EXPECT_CALL(tick, GetMs() ).WillRepeatedly(Return(0U) );
	EXPECT_CALL(twi, ReadRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );

	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_IDENTIFICATION_MODEL_ID, _) ).WillOnce(DoAll(SetArgReferee <2U>(0xEEU), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, 0x83, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(1U), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_RESULT_INTERRUPT_STATUS, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(0x07U), Return(true) ) );

	success = vl53l0x.Initialize();

	EXPECT_TRUE(Core::Utils::CoreStatusToBool(vl53l0x.SetThreshold(350U) ) );
	EXPECT_EQ(350U, vl53l0x.GetThreshold() );
	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}

TEST(ComponentVl53l0x, IsDetecting_UnderThreshold)
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick> tick;
	StrictMock <MockTwi>  twi;

	Vl53l0x vl53l0x(twi, tick);

	EXPECT_CALL(tick, GetMs() ).WillRepeatedly(Return(0U) );
	EXPECT_CALL(twi, ReadRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );

	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_IDENTIFICATION_MODEL_ID, _) ).WillOnce(DoAll(SetArgReferee <2U>(0xEEU), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, 0x83, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(1U), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_RESULT_INTERRUPT_STATUS, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(0x07U), Return(true) ) );

	success = vl53l0x.Initialize();

	EXPECT_CALL(twi, ReadRegister16Bits(_, VL53L0X_RESULT_RANGE_STATUS + 10, _) ).WillOnce(DoAll(SetArgReferee <2U>(250U), Return(true) ) );

	EXPECT_EQ(250U, vl53l0x.GetDistance() );
	EXPECT_TRUE(vl53l0x.IsDetecting() );
	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}

TEST(ComponentVl53l0x, IsDetecting_AboveThreshold)
{
	Core::CoreStatus      success = Core::CoreStatus::CORE_ERROR;
	StrictMock <MockTick> tick;
	StrictMock <MockTwi>  twi;

	Vl53l0x vl53l0x(twi, tick);

	EXPECT_CALL(tick, GetMs() ).WillRepeatedly(Return(0U) );
	EXPECT_CALL(twi, ReadRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegisters(_, _, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, ReadRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );
	EXPECT_CALL(twi, WriteRegister16Bits(_, _, _) ).WillRepeatedly(Return(true) );

	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_IDENTIFICATION_MODEL_ID, _) ).WillOnce(DoAll(SetArgReferee <2U>(0xEEU), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, 0x83, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(1U), Return(true) ) );
	EXPECT_CALL(twi, ReadRegister(_, VL53L0X_RESULT_INTERRUPT_STATUS, _) ).WillRepeatedly(DoAll(SetArgReferee <2U>(0x07U), Return(true) ) );

	success = vl53l0x.Initialize();

	EXPECT_CALL(twi, ReadRegister16Bits(_, VL53L0X_RESULT_RANGE_STATUS + 10, _) ).WillOnce(DoAll(SetArgReferee <2U>(350U), Return(true) ) );

	EXPECT_EQ(350U, vl53l0x.GetDistance() );
	EXPECT_FALSE(vl53l0x.IsDetecting() );
	EXPECT_TRUE(Core::Utils::CoreStatusToBool(success) );
}
