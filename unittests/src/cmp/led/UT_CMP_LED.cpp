#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockGpio.h"
#include "../../../../src/cmp/Led.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

using namespace Component;


TEST(ComponentLed, Initialize_Ok)
{
	bool success = false;
	StrictMock <MockGpio> gpio;
	Led led(gpio);

	EXPECT_CALL(gpio, Set() ).WillOnce(Return(true) );

	success = led.Initialize();

	EXPECT_TRUE(success);
}

TEST(ComponentLed, On)
{
	bool success = false;
	StrictMock <MockGpio> gpio;
	Led led(gpio);

	EXPECT_CALL(gpio, Set() ).WillOnce(Return(true) );
	EXPECT_CALL(gpio, Reset() ).WillOnce(Return(true) );

	success = led.Initialize();
	led.On();

	Led::LedState status = led.Get();

	EXPECT_TRUE(success);
	EXPECT_EQ(status, Led::LedState::ON);
}

TEST(ComponentLed, Off)
{
	bool success = false;
	StrictMock <MockGpio> gpio;
	Led led(gpio);

	EXPECT_CALL(gpio, Set() ).Times(2U).WillRepeatedly(Return(true) );

	success = led.Initialize();
	led.Off();

	Led::LedState status = led.Get();

	EXPECT_TRUE(success);
	EXPECT_EQ(status, Led::LedState::OFF);
}

TEST(ComponentLed, Toggle)
{
	bool success = false;
	StrictMock <MockGpio> gpio;
	Led led(gpio);

	EXPECT_CALL(gpio, Set() ).WillOnce(Return(true) );
	EXPECT_CALL(gpio, Reset() ).WillOnce(Return(true) );

	success = led.Initialize();
	led.Toggle();

	Led::LedState status = led.Get();

	EXPECT_TRUE(success);
	EXPECT_EQ(status, Led::LedState::ON);
}

TEST(ComponentLed, Get)
{
	bool success = false;
	StrictMock <MockGpio> gpio;
	Led led(gpio);

	EXPECT_CALL(gpio, Set() ).WillOnce(Return(true) );

	success = led.Initialize();
	Led::LedState status = led.Get();

	EXPECT_TRUE(success);
	EXPECT_EQ(status, Led::LedState::OFF);
}
