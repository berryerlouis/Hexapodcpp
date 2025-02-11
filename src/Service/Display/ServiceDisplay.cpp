#include "ServiceDisplay.h"

namespace Service
{
    namespace Display
    {
        ServiceDisplay::ServiceDisplay(Ssd1306Interface &ssd1306
                                       , ButtonInterface &button
                                       , SoundInterface &soundInterfaceLeft
                                       , SoundInterface &soundInterfaceRight
                                       , SensorProximityMultipleInterface &sensors
                                       , Event::MessageInterface &messageListener) :
            Service(DISPLAY, 20U, messageListener)
            , mSsd1306(ssd1306)
            , mButton(button)
            , mSoundInterfaceLeft(soundInterfaceLeft)
            , mSoundInterfaceRight(soundInterfaceRight)
            , mSensors(sensors)
            , mBmpBatteryLevel{.bmp = const_cast<uint8_t *>(Bitmaps::Battery0), .width = 16U, .height = 7U}
            , mBmpCommunication{.bmp = const_cast<uint8_t *>(Bitmaps::Communication), .width = 16U, .height = 8U}
            , mBmpProximity{.bmp = const_cast<uint8_t *>(Bitmaps::ArrowCenter), .width = 16U, .height = 6U}
            , mBmpButton{.bmp = const_cast<uint8_t *>(Bitmaps::ButtonRelease), .width = 16U, .height = 7U}
            , mBmpSound{.bmp = const_cast<uint8_t *>(Bitmaps::SoundLeft), .width = 16U, .height = 7U}
            , mPreviousTime(0UL)
            , mToggleCommunicationBmp(false) {
        }

        Core::Status ServiceDisplay::Initialize(void) {
            Core::Status success = Core::Status::CORE_ERROR;
            if (this->mSsd1306.Initialize() == Core::Status::CORE_OK) {
                this->mButton.Attach(this);
                this->mSoundInterfaceLeft.Attach(this);
                this->mSoundInterfaceRight.Attach(this);
                this->mSensors.Attach(this);
                this->DisplayBackground();
                this->DisplayBatteryLevel(UNKNOWN);
                this->DisplayButtonBmp(RELEASE);
                this->mInitialized = true;
                success = Core::Status::CORE_OK;
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

        void ServiceDisplay::Notified(const ButtonStruct &button) {
            this->DisplayButtonBmp(button.state);
        }

        void ServiceDisplay::Notified(const SoundStruct &sound) {
            this->DisplaySound(sound.id, sound.state, sound.delay);
        }

        void ServiceDisplay::Notified(const SensorsStruct &sensor) {
            this->DisplayProximitySensor(sensor.id, sensor.distance);
        }


        void ServiceDisplay::DisplayBackground(void) const {
            this->mSsd1306.DrawLine(0U, 10U, SCREEN_WIDTH, 10U, Bitmaps::Color::COLOR_WHITE);
            this->mSsd1306.DrawLine(18U, 0U, 18U, SCREEN_HEIGHT, Bitmaps::Color::COLOR_WHITE);
            this->mSsd1306.DrawLine(SCREEN_WIDTH - 18U, 0U, SCREEN_WIDTH - 18U, SCREEN_HEIGHT,
                                    Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayButtonBmp(const ButtonState &buttonState) {
            this->mSsd1306.EraseArea(SCREEN_WIDTH - this->mBmpCommunication.width - 2U - this->mBmpButton.width, 0U,
                                     this->mBmpButton.width, 8U);
            if (buttonState == RELEASE) {
                this->mBmpButton.bmp = const_cast<uint8_t *>(Bitmaps::ButtonRelease);
            } else {
                this->mBmpButton.bmp = const_cast<uint8_t *>(Bitmaps::ButtonPush);
            }
            this->mSsd1306.DrawBitmap(&this->mBmpButton,
                                      SCREEN_WIDTH - this->mBmpCommunication.width - 2U - this->mBmpButton.width,
                                      0U,
                                      Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayCommunicationBmp(void) {
            if (this->mToggleCommunicationBmp == true) {
                this->mSsd1306.DrawBitmap(&this->mBmpCommunication, SCREEN_WIDTH - this->mBmpCommunication.width, 0U,
                                          Bitmaps::Color::COLOR_WHITE);
                this->mToggleCommunicationBmp = false;
            } else {
                this->mSsd1306.EraseArea(SCREEN_WIDTH - this->mBmpCommunication.width, 0U,
                                         this->mBmpCommunication.width, 8U);
                this->mToggleCommunicationBmp = true;
            }
        }

        void ServiceDisplay::DisplayBatteryLevel(const Battery::BatteryState state) {
            if (state == Battery::BatteryState::WARNING) {
                this->mBmpBatteryLevel.bmp = const_cast<uint8_t *>(Bitmaps::Battery50);
            } else if (state == Battery::BatteryState::NOMINAL) {
                this->mBmpBatteryLevel.bmp = const_cast<uint8_t *>(Bitmaps::Battery100);
            } else {
                this->mBmpBatteryLevel.bmp = const_cast<uint8_t *>(Bitmaps::Usb);
            }
            this->mSsd1306.DrawBitmap(&this->mBmpBatteryLevel, 0, 0, Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayProximitySensor(const Component::Proximity::SensorsId sensorId,
                                                    const uint16_t distance) {
            if (sensorId == Component::Proximity::SensorsId::SRF_LEFT) {
                if (distance > 30) {
                    this->mSsd1306.EraseArea((SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U) -
                                             this->mBmpProximity.width,
                                             0U, this->mBmpProximity.width, 8U);
                } else {
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowLeft);
                    this->mSsd1306.DrawBitmap(&this->mBmpProximity,
                                              (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U) -
                                              this->mBmpProximity.width,
                                              0U, Bitmaps::Color::COLOR_WHITE);
                }
            } else if (sensorId == Component::Proximity::SensorsId::VLX) {
                if (distance > 300) {
                    this->mSsd1306.EraseArea((SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U), 0U,
                                             this->mBmpProximity.width, 8U);
                } else {
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowUp);
                    this->mSsd1306.DrawBitmap(&this->mBmpProximity,
                                              (SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U), 0U,
                                              Bitmaps::Color::COLOR_WHITE);
                }
            } else if (sensorId == Component::Proximity::SensorsId::SRF_RIGHT) {
                if (distance > 30) {
                    this->mSsd1306.EraseArea((SCREEN_WIDTH / 2U) + (this->mBmpProximity.width / 2U), 0U,
                                             this->mBmpProximity.width, 8U);
                } else {
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowRight);
                    this->mSsd1306.DrawBitmap(&this->mBmpProximity,
                                              (SCREEN_WIDTH / 2U) + (this->mBmpProximity.width / 2U), 0U,
                                              Bitmaps::Color::COLOR_WHITE);
                }
            }
        }

        void ServiceDisplay::DisplaySound(const SoundId &soundId, const SoundState &soundState, const uint16_t period) {
            (void) period;
            if (soundId == SOUND_RIGHT) {
                if (soundState == NO_SOUND) {
                    this->mSsd1306.EraseArea((SCREEN_WIDTH) - (this->mBmpSound.width),
                                             12U, this->mBmpSound.width, 8U);
                } else {
                    this->mBmpSound.bmp = const_cast<uint8_t *>(Bitmaps::SoundRight);
                    this->mSsd1306.DrawBitmap(&this->mBmpSound, (SCREEN_WIDTH) - (this->mBmpSound.width),
                                              12U, Bitmaps::Color::COLOR_WHITE);
                }
            } else {
                if (soundState == NO_SOUND) {
                    this->mSsd1306.EraseArea(0U, 12U, this->mBmpSound.width, 8U);
                } else {
                    this->mBmpSound.bmp = const_cast<uint8_t *>(Bitmaps::SoundLeft);
                    this->mSsd1306.DrawBitmap(&this->mBmpSound, 0U, 12U,
                                              Bitmaps::Color::COLOR_WHITE);
                }
            }
        }

    } // namespace Display
} // namespace Service
