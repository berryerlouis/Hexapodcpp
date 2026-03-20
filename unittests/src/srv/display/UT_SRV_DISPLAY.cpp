#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../../../mock/cmp/MockButton.h"
#include "../../../mock/cmp/MockSensorProximity.h"
#include "../../../mock/cmp/MockSound.h"
#include "../../../mock/cmp/MockSsd1306.h"
#include "../../../mock/srv/MockEventDispatcherInterface.h"
#include "../../../mock/srv/MockMessageListener.h"

#include "../../../../src/Component/Button/ButtonState.h"
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
            UT_SRV_DISPLAY()
                : mMockSsd1306()
                , mMockEventDispatcherInterface()
                , mMockMessageInterface()
                , mServiceDisplay(mMockSsd1306,
                                  mMockMessageInterface,
                                  mMockEventDispatcherInterface) {
            }

            virtual void SetUp() {
                EXPECT_CALL(mMockSsd1306, Initialize()).WillOnce(Return(Core::Status::CORE_ERROR));
                EXPECT_EQ(Core::Status::CORE_ERROR, mServiceDisplay.Initialize());

                EXPECT_CALL(mMockSsd1306, Initialize()).WillOnce(Return(Core::Status::CORE_OK));
                EXPECT_CALL(
                        mMockSsd1306,
                        DrawLine(0, 10U, SCREEN_WIDTH, 10U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                        .Times(1U);
                EXPECT_CALL(
                        mMockSsd1306,
                        DrawLine(18U, 10U, 18U, SCREEN_HEIGHT, Bitmap::Bitmaps::Color::COLOR_WHITE))
                        .Times(1U);
                EXPECT_CALL(mMockSsd1306,
                            DrawLine(SCREEN_WIDTH - 18U,
                                     10U,
                                     SCREEN_WIDTH - 18U,
                                     SCREEN_HEIGHT,
                                     Bitmap::Bitmaps::Color ::COLOR_WHITE))
                        .Times(1U);
                EXPECT_CALL(mMockSsd1306,
                            DrawBitmap(_, 0U, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                        .Times(1U);
                EXPECT_CALL(mMockSsd1306, EraseArea(94U, 0U, 16U, 8U)).Times(1U);
                EXPECT_CALL(mMockSsd1306,
                            DrawBitmap(_, 94U, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                        .Times(1U);

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


        TEST_F(UT_SRV_DISPLAY, Update_Ok) {
            bool toggle = false;

            for (uint64_t i = 500U; i < 10U * 500U; i += 500U) {
                if (toggle == true) {
                    toggle = false;
                    EXPECT_CALL(mMockSsd1306,
                                DrawBitmap(_, _, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                            .Times(1U);
                } else {
                    toggle = true;
                    EXPECT_CALL(mMockSsd1306, EraseArea(_, 0U, _, 8U)).Times(1U);
                }
                EXPECT_CALL(mMockSsd1306, Update(i)).Times(1U);
                mServiceDisplay.Update(i);
            }
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_Button) {
            const Component::Button::ButtonStruct button(
                    {.state = Component::Button::ButtonState::PUSH, .delayMs = 10U});
            const Event::Event event = Event::Event(BUTTON, EventType::EVENT_BUTTON_UPDATE, button);

            EXPECT_CALL(mMockSsd1306, EraseArea(94U, 0U, 16U, 8U)).Times(1U);
            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 94U, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_Comm) {
            const Component::Communication::CommunicationStruct com(CLIENT_CONNECTED);
            const Event::Event                                  event =
                    Event::Event(COMMUNICATION, EventType::EVENT_COM_UPDATE, com);

            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 112U, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_Battery) {
            const Component::Battery::BatteryStruct battery(
                    {.state = NOMINAL, .voltage = 0U, .intensity = 0U});
            const Event::Event event =
                    Event::Event(BATTERY, EventType::EVENT_BATTERY_UPDATE, battery);

            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 0U, 0U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_SoundLeft) {
            const Component::Sound::SoundStruct soundStruct({.id = SOUND_LEFT, .delay = 0U});
            const Event::Event                  event =
                    Event::Event(SOUND, EventType::EVENT_SOUND_UPDATE, soundStruct);

            EXPECT_CALL(mMockSsd1306, EraseArea(0U, 12U, 16U, 8U)).Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_SoundLeftHit) {
            const Component::Sound::SoundStruct soundStruct({.id = SOUND_LEFT, .delay = 10U});
            const Event::Event                  event =
                    Event::Event(SOUND, EventType::EVENT_SOUND_UPDATE, soundStruct);

            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 0U, 12U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_SoundRight) {
            const Component::Sound::SoundStruct soundStruct({.id = SOUND_RIGHT, .delay = 0U});
            const Event::Event                  event =
                    Event::Event(SOUND, EventType::EVENT_SOUND_UPDATE, soundStruct);

            EXPECT_CALL(mMockSsd1306, EraseArea(112U, 12U, 16U, 8U)).Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_SoundRightHit) {
            const Component::Sound::SoundStruct soundStruct({.id = SOUND_RIGHT, .delay = 10U});
            const Event::Event                  event =
                    Event::Event(SOUND, EventType::EVENT_SOUND_UPDATE, soundStruct);

            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 112U, 12U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_ProximityUSLeft) {
            const Component::Proximity::SensorsStruct proxStruct({.id = SRF_LEFT, .distance = 10U, .distanceArray = {}});
            const Event::Event                        event =
                    Event::Event(PROXIMITY, EventType::EVENT_SENSOR_UPDATE, proxStruct);

            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 0U, 22U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_ProximityUSRight) {
            const Component::Proximity::SensorsStruct proxStruct(
                    {.id = SRF_RIGHT, .distance = 10U, .distanceArray = {}});
            const Event::Event event =
                    Event::Event(PROXIMITY, EventType::EVENT_SENSOR_UPDATE, proxStruct);

            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 112U, 22U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_ProximityLaser) {
            const Component::Proximity::SensorsStruct proxStruct({.id = VLX, .distance = 10U, .distanceArray = {}});
            const Event::Event                        event =
                    Event::Event(PROXIMITY, EventType::EVENT_SENSOR_UPDATE, proxStruct);

            EXPECT_CALL(mMockSsd1306, DrawBitmap(_, 56U, 22U, Bitmap::Bitmaps::Color::COLOR_WHITE))
                    .Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_ProximityUSLeftFar) {
            const Component::Proximity::SensorsStruct proxStruct({.id = SRF_LEFT, .distance = 40U, .distanceArray = {}});
            const Event::Event                        event =
                    Event::Event(PROXIMITY, EventType::EVENT_SENSOR_UPDATE, proxStruct);

            EXPECT_CALL(mMockSsd1306, EraseArea(0U, 22U, 16U, 8U)).Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_ProximityUSRightFar) {
            const Component::Proximity::SensorsStruct proxStruct(
                    {.id = SRF_RIGHT, .distance = 40U, .distanceArray = {}});
            const Event::Event event =
                    Event::Event(PROXIMITY, EventType::EVENT_SENSOR_UPDATE, proxStruct);

            EXPECT_CALL(mMockSsd1306, EraseArea(112U, 22U, 16U, 8U)).Times(1U);
            mServiceDisplay.OnEvent(event);
        }

        TEST_F(UT_SRV_DISPLAY, OnEvent_ProximityLaserFar) {
            const Component::Proximity::SensorsStruct proxStruct({.id = VLX, .distance = 400U, .distanceArray = {}});
            const Event::Event                        event =
                    Event::Event(PROXIMITY, EventType::EVENT_SENSOR_UPDATE, proxStruct);

            EXPECT_CALL(mMockSsd1306, EraseArea(56U, 22U, 16U, 8U)).Times(1U);
            mServiceDisplay.OnEvent(event);
        }
    } // namespace Display
} // namespace Service
