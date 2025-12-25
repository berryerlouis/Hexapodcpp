#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockButton.h"
#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../mock/cmp/MockSound.h"
#include "../../../mock/cmp/MockSsd1306.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Component/Proximity/SensorProximity.h"
#include "../../../../src/Service/Display/ServiceDisplay.h"
#include "../../../mock/cmp/MockBattery.h"
#include "../../../mock/cmp/MockCommunication.h"

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

namespace Service
{
    namespace Display
    {
        class UT_SRV_DISPLAY : public ::testing::Test {
        protected:
            UT_SRV_DISPLAY() :
                mMockSsd1306(),
                mMockEventDispatcherInterface(),
                mMockMessageInterface(),
                mServiceDisplay(mMockSsd1306,
                                mMockMessageInterface,
                                mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockSsd1306, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceDisplay.Initialize());

                EXPECT_CALL(mMockSsd1306, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(mMockSsd1306, DrawLine(0, 10U, SCREEN_WIDTH, 10U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                        .Times(1U);
                EXPECT_CALL(mMockSsd1306, DrawLine(18U, 10U, 18U, SCREEN_HEIGHT, Bitmap::Bitmaps::Color::COLOR_WHITE))
                        .Times(1U);
                EXPECT_CALL(mMockSsd1306,
                            DrawLine(SCREEN_WIDTH - 18U,
                                     10U,
                                     SCREEN_WIDTH - 18U,
                                     SCREEN_HEIGHT,
                                     Bitmap::Bitmaps::Color ::COLOR_WHITE))
                        .Times(1U);
                EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 0U, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE)).Times(1U);
                EXPECT_CALL(mMockSsd1306, EraseArea(94U, 0U, 16U, 8U)).Times(1U);
                EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 94U, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE)).Times(1U);

                EXPECT_CALL(mMockEventDispatcherInterface, AddListener(_));
                EXPECT_EQ(Core::Status::CORE_OK, mServiceDisplay.Initialize());
            }

            virtual void TearDown() {
            }

            virtual ~UT_SRV_DISPLAY() = default;

            /* Mocks */
            StrictMock<Component::Display::MockSsd1306>     mMockSsd1306;
            StrictMock<Event::MockEventDispatcherInterface> mMockEventDispatcherInterface;
            StrictMock<Message::MockMessageInterface>       mMockMessageInterface;

            /* Test class */
            ServiceDisplay mServiceDisplay;
        };


        TEST_F(UT_SRV_DISPLAY,
               Update_Ok) {
            bool toggle = false;

            for (uint64_t i = 500U; i < 10U * 500U; i += 500U) {
                if (toggle == true) {
                    toggle = false;
                    EXPECT_CALL(mMockSsd1306, DrawBitmap(_, _, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE)).Times(1U);
                } else {
                    toggle = true;
                    EXPECT_CALL(mMockSsd1306, EraseArea(_, 0U, _, 8U)).Times(1U);
                }
                EXPECT_CALL(mMockSsd1306, Update(i)).Times(1U);
                mServiceDisplay.Update(i);
            }
        }
    } // namespace Display
} // namespace Service
