#include "ServiceDisplay.h"

namespace Service
{
    namespace Display
    {
        ServiceDisplay::ServiceDisplay(Ssd1306Interface &ssd1306, BatteryInterface &batteryInterface,
                                       SensorProximityMultipleInterface &proximity)
            : Service(5U)
              , mSsd1306(ssd1306)
              , mBatteryInterface(batteryInterface)
              , mProximity(proximity)
              , mBmpBatteryLevel{.bmp = (uint8_t *) Bitmaps::Battery0, .width = 16U, .height = 7U}
              , mBmpCommunication{.bmp = (uint8_t *) Bitmaps::Communication, .width = 16U, .height = 8U}
              , mBmpProximity{.bmp = (uint8_t *) Bitmaps::ArrowCenter, .width = 16U, .height = 6U}
              , mPreviousTime(0UL)
              , mToggleCommunicationBmp(false) {
        }

        Core::CoreStatus ServiceDisplay::Initialize(void) {
            Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
            if (true == this->mSsd1306.Initialize()) {
                this->mBatteryInterface.Attach(this);
                this->mProximity.Attach(this);
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

        void ServiceDisplay::DisplayBackground(void) {
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

        void ServiceDisplay::DisplayBatteryLevel(BatteryState state) {
            if (state == BatteryState::WARNING) {
                this->mBmpBatteryLevel.bmp = (uint8_t *) Bitmaps::Battery50;
            } else if (state == BatteryState::NOMINAL) {
                this->mBmpBatteryLevel.bmp = (uint8_t *) Bitmaps::Battery100;
            } else {
                this->mBmpBatteryLevel.bmp = (uint8_t *) Bitmaps::Battery0;
            }
            this->mSsd1306.DrawBitmap(&this->mBmpBatteryLevel, 0, 0, Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayProximitySensor(SensorsId sensor) {
            if (sensor == SensorsId::SRF_LEFT) {
                this->mBmpProximity.bmp = (uint8_t *) Bitmaps::ArrowLeft;

                this->mSsd1306.DrawBitmap(
                    &this->mBmpProximity,
                    (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U) - this->mBmpProximity.width,
                    0U,
                    Bitmaps::Color::COLOR_WHITE);
            }
            if (sensor == SensorsId::VLX) {
                this->mBmpProximity.bmp = (uint8_t *) Bitmaps::ArrowUp;
                this->mSsd1306.DrawBitmap(
                    &this->mBmpProximity,
                    (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U),
                    0U,
                    Bitmaps::Color::COLOR_WHITE);
            }
            if (sensor == SensorsId::SRF_RIGHT) {
                this->mBmpProximity.bmp = (uint8_t *) Bitmaps::ArrowRight;
                this->mSsd1306.DrawBitmap(
                    &this->mBmpProximity,
                    (SCREEN_WIDTH / 2U) + (this->mBmpProximity.width / 2U),
                    0U,
                    Bitmaps::Color::COLOR_WHITE);
            }
        }

        void ServiceDisplay::UpdatedBatteryState(const BatteryState &batteryState) {
            this->DisplayBatteryLevel(batteryState);
        }

        void ServiceDisplay::Detect(const SensorsId &sensorId) {
            this->DisplayProximitySensor(sensorId);
        }

        void ServiceDisplay::NoDetect(const SensorsId &sensorId) {
            if (sensorId == SensorsId::SRF_LEFT) {
                this->mSsd1306.EraseArea(
                    (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U) - this->mBmpProximity.width,
                    0U,
                    this->mBmpProximity.width,
                    8U);
            }
            if (sensorId == SensorsId::VLX) {
                this->mSsd1306.EraseArea(
                    (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U),
                    0U,
                    this->mBmpProximity.width,
                    8U);
            }
            if (sensorId == SensorsId::SRF_RIGHT) {
                this->mSsd1306.EraseArea(
                    (SCREEN_WIDTH / 2U) + (this->mBmpProximity.width / 2U),
                    0U,
                    this->mBmpProximity.width,
                    8U);
            }
        }
    }
}
