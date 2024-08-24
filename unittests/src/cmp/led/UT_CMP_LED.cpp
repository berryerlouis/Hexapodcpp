#include <gmock/gmock.h>
#include <gtest/gtest.h>


#include "../../../mock/drv/MockGpio.h"
#include "../../../../src/Component/Led/Led.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Component {
namespace Led {
class UT_CMP_LED : public ::testing::Test {
protected:
	UT_CMP_LED() :
		mMockGpio(),
		mLed( mMockGpio )
	{
	}

	virtual void SetUp ()
	{
	}

	virtual void TearDown ()
	{
	}

	virtual ~UT_CMP_LED() = default;

	/* Mocks */
	StrictMock <Driver::Gpio::MockGpio> mMockGpio;

	/* Test class */
	Led mLed;
};

TEST_F( UT_CMP_LED, Initialize_Ok )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockGpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mLed.Initialize();

	EXPECT_TRUE( success );
}

TEST_F( UT_CMP_LED, On )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockGpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockGpio, Reset() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mLed.Initialize();
	mLed.On();

	Led::LedState status = mLed.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::ON );
}

TEST_F( UT_CMP_LED, Off )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockGpio, Set() ).Times( 2U ).WillRepeatedly( Return( Core::CoreStatus::CORE_OK ) );

	success = mLed.Initialize();
	mLed.Off();

	Led::LedState status = mLed.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::OFF );
}

TEST_F( UT_CMP_LED, Toggle )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockGpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );
	EXPECT_CALL( mMockGpio, Reset() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mLed.Initialize();
	mLed.Toggle();

	Led::LedState status = mLed.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::ON );
}

TEST_F( UT_CMP_LED, Get )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	EXPECT_CALL( mMockGpio, Set() ).WillOnce( Return( Core::CoreStatus::CORE_OK ) );

	success = mLed.Initialize();
	Led::LedState status = mLed.Get();

	EXPECT_TRUE( success );
	EXPECT_EQ( status, Led::LedState::OFF );
}
}
}
