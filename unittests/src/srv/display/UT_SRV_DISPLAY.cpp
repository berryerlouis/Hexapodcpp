#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockSsd1306.h"
#include "../../../mock/srv/MockEventListener.h"

#include "../../../../src/Service/Display/ServiceDisplay.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
	namespace Display
	{
		class UT_SRV_DISPLAY : public ::testing::Test {
		protected:
			UT_SRV_DISPLAY() : mMockSsd1306(),
			                   mMockEventListener(),
			                   mServiceDisplay(mMockSsd1306, mMockEventListener) {
			}

			virtual void SetUp() {
				EXPECT_CALL(mMockSsd1306, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_ERROR));
				EXPECT_EQ(Core::CoreStatus::CORE_ERROR, mServiceDisplay.Initialize());

				EXPECT_CALL(mMockSsd1306, Initialize()).WillOnce(Return(Core::CoreStatus::CORE_OK));
				EXPECT_CALL(mMockSsd1306,
				            DrawLine( 0, 10U, SCREEN_WIDTH, 10U, Bitmap::Bitmaps::Color::COLOR_WHITE )).Times(1U);
				EXPECT_EQ(Core::CoreStatus::CORE_OK, mServiceDisplay.Initialize());
			}

			virtual void TearDown() {
			}

			virtual ~UT_SRV_DISPLAY() = default;

			/* Mocks */
			StrictMock<Component::Display::MockSsd1306> mMockSsd1306;
			StrictMock<Event::MockEventListener> mMockEventListener;

			/* Test class */
			ServiceDisplay mServiceDisplay;
		};


		TEST_F(UT_SRV_DISPLAY, Update_Ok) {
			bool toggle = false;

			for (uint64_t i = 500U; i < 10U * 500U; i += 500U) {
				if (toggle == true) {
					toggle = false;
					EXPECT_CALL(mMockSsd1306, DrawBitmap( _, _, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE )).Times(1U);
				} else {
					toggle = true;
					EXPECT_CALL(mMockSsd1306, EraseArea( _, 0U, _, 8U )).Times(1U);
				}
				EXPECT_CALL(mMockSsd1306, Update( i )).Times(1U);
				mServiceDisplay.Update(i);
			}
		}
	}
}
