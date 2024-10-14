#include "ServiceDisplay.h"

namespace Service
{
    namespace Display
    {
        ServiceDisplay::ServiceDisplay(Ssd1306Interface &ssd1306, Event::EventListenerInterface &eventListener)
            : Service(20U, eventListener)
              , mSsd1306(ssd1306)
              , mBmpBatteryLevel{.bmp = const_cast<uint8_t *>(Bitmaps::Battery0), .width = 16U, .height = 7U}
              , mBmpCommunication{.bmp = const_cast<uint8_t *>(Bitmaps::Communication), .width = 16U, .height = 8U}
              , mBmpProximity{.bmp = const_cast<uint8_t *>(Bitmaps::ArrowCenter), .width = 16U, .height = 6U}
              , mPreviousTime(0UL)
              , mToggleCommunicationBmp(false) {
        }

        Core::CoreStatus ServiceDisplay::Initialize(void) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            if (this->mSsd1306.Initialize() == Core::CoreStatus::CORE_OK) {
                this->DisplayBackground();
                success = Core::CoreStatus::CORE_OK;
            }
            return (success);
        }

        void ServiceDisplay::Update(const uint64_t currentTime) {
            if ((currentTime - this->mPreviousTime) >= 500UL) {
                this->DisplayCommunicationBmp();
                mPreviousTime = currentTime;
            }
            this->mSsd1306.Update(currentTime);
        }

        void ServiceDisplay::DispatchEvent(const SEvent &event) {
            if (event.id == EServices::BATTERY) {
                this->DisplayBatteryLevel(static_cast<Battery::BatteryState>(event.value));
            } else if (event.id == EServices::PROXIMITY) {
                const uint16_t distance = PTR_TO_UINT16(&event.params[0U]);
                this->DisplayProximitySensor(static_cast<Proximity::SensorsId>(event.value), distance);
            }
        }

        void ServiceDisplay::DisplayBackground(void) const {
            this->mSsd1306.DrawLine(0, 10U, SCREEN_WIDTH, 10U, Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayCommunicationBmp(void) {
            if (this->mToggleCommunicationBmp == true) {
                this->mSsd1306.DrawBitmap(
                    &this->mBmpCommunication,
                    SCREEN_WIDTH - this->mBmpCommunication.width,
                    0U,
                    Bitmaps::Color::COLOR_WHITE);
                this->mToggleCommunicationBmp = false;
            } else {
                this->mSsd1306.EraseArea(
                    SCREEN_WIDTH - this->mBmpCommunication.width,
                    0U,
                    this->mBmpCommunication.width,
                    8U);
                this->mToggleCommunicationBmp = true;
            }
        }

        void ServiceDisplay::DisplayBatteryLevel(const Battery::BatteryState state) {
            if (state == Battery::BatteryState::WARNING) {
                this->mBmpBatteryLevel.bmp = const_cast<uint8_t *>(Bitmaps::Battery50);
            } else if (state == Battery::BatteryState::NOMINAL) {
                this->mBmpBatteryLevel.bmp = const_cast<uint8_t *>(Bitmaps::Battery100);
            } else {
                this->mBmpBatteryLevel.bmp = const_cast<uint8_t *>(Bitmaps::Battery0);
            }
            this->mSsd1306.DrawBitmap(&this->mBmpBatteryLevel, 0, 0, Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayProximitySensor(const Component::Proximity::SensorsId sensorId,
                                                    const uint16_t distance) {
            if (sensorId == Component::Proximity::SensorsId::SRF_LEFT) {
                if (distance > 30) {
                    this->mSsd1306.EraseArea(
                        (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U) - this->mBmpProximity.width,
                        0U,
                        this->mBmpProximity.width,
                        8U);
                } else {
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowLeft);
                    this->mSsd1306.DrawBitmap(
                        &this->mBmpProximity,
                        (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U) - this->mBmpProximity.width,
                        0U,
                        Bitmaps::Color::COLOR_WHITE);
                }
            } else if (sensorId == Component::Proximity::SensorsId::VLX) {
                if (distance > 300) {
                    this->mSsd1306.EraseArea(
                        (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U),
                        0U,
                        this->mBmpProximity.width,
                        8U);
                } else {
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowUp);
                    this->mSsd1306.DrawBitmap(
                        &this->mBmpProximity,
                        (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U),
                        0U,
                        Bitmaps::Color::COLOR_WHITE);
                }
            } else if (sensorId == Component::Proximity::SensorsId::SRF_RIGHT) {
                if (distance > 30) {
                    this->mSsd1306.EraseArea(
                        (SCREEN_WIDTH / 2U) + (this->mBmpProximity.width / 2U),
                        0U,
                        this->mBmpProximity.width,
                        8U);
                } else {
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowRight);
                    this->mSsd1306.DrawBitmap(
                        &this->mBmpProximity,
                        (SCREEN_WIDTH / 2U) + (this->mBmpProximity.width / 2U),
                        0U,
                        Bitmaps::Color::COLOR_WHITE);
                }
            }
        }
    }
}
