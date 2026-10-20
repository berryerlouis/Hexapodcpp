#include "ServiceDisplay.h"


namespace Service
{
    namespace Display
    {
        ServiceDisplay::ServiceDisplay(Ssd1306Interface                &ssd1306,
                                       Message::MessageInterface       &messageListener,
                                       Event::EventDispatcherInterface &eventDispatcher)
            : Service(DISPLAY, 25U, messageListener, eventDispatcher)
            , mSsd1306(ssd1306)
            , mBmpBatteryLevel{.bmp = const_cast<uint8_t *>(Bitmaps::Battery0),
                               .width = 16U,
                               .height = 7U}
            , mBmpCommunication{.bmp = const_cast<uint8_t *>(Bitmaps::Communication),
                                .width = 16U,
                                .height = 8U}
            , mBmpGamepad{.bmp = const_cast<uint8_t *>(Bitmaps::Gamepad),
                           .width = 16U,
                           .height = 8U}
            , mBmpProximity{.bmp = const_cast<uint8_t *>(Bitmaps::ArrowCenter),
                            .width = 16U,
                            .height = 6U}
            , mBmpButton{.bmp = const_cast<uint8_t *>(Bitmaps::ButtonRelease),
                         .width = 16U,
                         .height = 7U}
            , mBmpSound{.bmp = const_cast<uint8_t *>(Bitmaps::SoundLeft),
                        .width = 16U,
                        .height = 7U}
            , mPreviousTime(0UL)
            , mToggleCommunicationBmp(false)
            , mState(NO_CLIENT)
            , mNotifiedTimeProximityUsLeft(0UL)
            , mNotifiedTimeProximityUsRight(0UL)
            , mNotifiedTimeProximityLaser(0UL)
            , mNotifiedTimeSoundLeft(0UL)
            , mNotifiedTimeSoundRight(0UL) {
        }

        Core::Status ServiceDisplay::Initialize() {
            Core::Status success = Core::Status::CORE_ERROR;
            if (this->mSsd1306.Initialize() == Core::Status::CORE_OK) {
                this->GetEventDispatcher().AddListener(this);
                this->DisplayBackground();
                this->DisplayBatteryLevel(UNKNOWN);
                this->DisplayButtonBmp(Button::ButtonState::RELEASE);
                this->DisplayGamepadState(GamepadConnectedState{false});
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
            if ((this->mNotifiedTimeSoundLeft > 0U) &&
                ((currentTime - this->mNotifiedTimeSoundLeft) >= 1000UL)) {
                this->DisplaySound({Component::Sound::SOUND_LEFT, 0U});
            }
            if ((this->mNotifiedTimeSoundRight > 0U) &&
                ((currentTime - this->mNotifiedTimeSoundRight) >= 1000UL)) {
                this->DisplaySound({Component::Sound::SOUND_RIGHT, 0U});
            }
            if ((this->mNotifiedTimeProximityLaser > 0U) &&
                ((currentTime - this->mNotifiedTimeProximityLaser) >= 1000UL)) {
                this->DisplayProximitySensor(Component::Proximity::SensorsId::VLX, 0xFFFFU);
            }
            if ((this->mNotifiedTimeProximityUsLeft > 0U) &&
                ((currentTime - this->mNotifiedTimeProximityUsLeft) >= 1000UL)) {
                this->DisplayProximitySensor(Component::Proximity::SensorsId::SRF_LEFT, 0xFFFFU);
            }
            if ((this->mNotifiedTimeProximityUsRight > 0U) &&
                ((currentTime - this->mNotifiedTimeProximityUsRight) >= 1000UL)) {
                this->DisplayProximitySensor(Component::Proximity::SensorsId::SRF_RIGHT, 0xFFFFU);
            }

            this->mSsd1306.Update(currentTime);
        }

        void ServiceDisplay::DisplayBackground() const {
            this->mSsd1306.DrawLine(0U, 10U, SCREEN_WIDTH, 10U, Bitmaps::Color::COLOR_WHITE);
            this->mSsd1306.DrawLine(18U, 10U, 18U, SCREEN_HEIGHT, Bitmaps::Color::COLOR_WHITE);
            this->mSsd1306.DrawLine(SCREEN_WIDTH - 18U,
                                    10U,
                                    SCREEN_WIDTH - 18U,
                                    SCREEN_HEIGHT,
                                    Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayButtonBmp(const Button::ButtonState &buttonState) {
            this->mSsd1306.EraseArea(SCREEN_WIDTH - this->mBmpCommunication.width - 2U -
                                             this->mBmpButton.width,
                                     0U,
                                     this->mBmpButton.width,
                                     8U);
            if (buttonState == Button::ButtonState::RELEASE) {
                this->mBmpButton.bmp = const_cast<uint8_t *>(Bitmaps::ButtonRelease);
            } else {
                this->mBmpButton.bmp = const_cast<uint8_t *>(Bitmaps::ButtonPush);
            }
            this->mSsd1306.DrawBitmap(&this->mBmpButton,
                                      SCREEN_WIDTH - this->mBmpCommunication.width - 2U -
                                              this->mBmpButton.width,
                                      0U,
                                      Bitmaps::Color::COLOR_WHITE);
        }

        void ServiceDisplay::DisplayCommunicationBmp() {
            if (this->mState == NO_CLIENT) {
                if (this->mToggleCommunicationBmp) {
                    this->mSsd1306.DrawBitmap(&this->mBmpCommunication,
                                              SCREEN_WIDTH - this->mBmpCommunication.width,
                                              0U,
                                              Bitmaps::Color::COLOR_WHITE);
                    this->mToggleCommunicationBmp = false;
                } else {
                    this->mSsd1306.EraseArea(SCREEN_WIDTH - this->mBmpCommunication.width,
                                             0U,
                                             this->mBmpCommunication.width,
                                             8U);
                    this->mToggleCommunicationBmp = true;
                }
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
                if (distance > 30U) {
                    this->mNotifiedTimeProximityUsLeft = 0U;
                    this->mSsd1306.EraseArea(
                            0U, SCREEN_HEIGHT - 10U, this->mBmpProximity.width, 8U);
                } else {
                    this->mNotifiedTimeProximityUsLeft = Timer::Tick::GetInstance().GetMs();
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowLeft);
                    this->mSsd1306.DrawBitmap(&this->mBmpProximity,
                                              0U,
                                              SCREEN_HEIGHT - 10U,
                                              Bitmaps::Color::COLOR_WHITE);
                }
            } else if (sensorId == Component::Proximity::SensorsId::VLX) {
                if (distance > 300U) {
                    this->mNotifiedTimeProximityLaser = 0U;
                    this->mSsd1306.EraseArea((SCREEN_WIDTH / 2U) - (this->mBmpProximity.width / 2U),
                                             SCREEN_HEIGHT - 10U,
                                             this->mBmpProximity.width,
                                             8U);
                } else {
                    this->mNotifiedTimeProximityLaser = Timer::Tick::GetInstance().GetMs();
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowUp);
                    this->mSsd1306.DrawBitmap(&this->mBmpProximity,
                                              (SCREEN_WIDTH / 2U) -
                                                      (this->mBmpProximity.width / 2U),
                                              SCREEN_HEIGHT - 10U,
                                              Bitmaps::Color::COLOR_WHITE);
                }
            } else if (sensorId == Component::Proximity::SensorsId::SRF_RIGHT) {
                if (distance > 30U) {
                    this->mNotifiedTimeProximityUsRight = 0U;
                    this->mSsd1306.EraseArea((SCREEN_WIDTH - this->mBmpProximity.width),
                                             SCREEN_HEIGHT - 10U,
                                             this->mBmpProximity.width,
                                             8U);
                } else {
                    this->mNotifiedTimeProximityUsRight = Timer::Tick::GetInstance().GetMs();
                    this->mBmpProximity.bmp = const_cast<uint8_t *>(Bitmaps::ArrowRight);
                    this->mSsd1306.DrawBitmap(&this->mBmpProximity,
                                              (SCREEN_WIDTH - this->mBmpProximity.width),
                                              SCREEN_HEIGHT - 10U,
                                              Bitmaps::Color::COLOR_WHITE);
                }
            }
        }

        void ServiceDisplay::DisplaySound(const SoundStruct &soundStruct) {
            if (soundStruct.id == SOUND_RIGHT) {
                if (soundStruct.delay > 0U) {
                    this->mNotifiedTimeSoundRight = Timer::Tick::GetInstance().GetMs();
                    this->mBmpSound.bmp = const_cast<uint8_t *>(Bitmaps::SoundRight);
                    this->mSsd1306.DrawBitmap(&this->mBmpSound,
                                              (SCREEN_WIDTH) - (this->mBmpSound.width),
                                              12U,
                                              Bitmaps::Color::COLOR_WHITE);
                } else {
                    this->mNotifiedTimeSoundRight = 0U;
                    this->mSsd1306.EraseArea((SCREEN_WIDTH) - (this->mBmpSound.width),
                                             12U,
                                             this->mBmpSound.width,
                                             8U);
                }
            } else if (soundStruct.id == SOUND_LEFT) {
                if (soundStruct.delay > 0U) {
                    this->mNotifiedTimeSoundLeft = Timer::Tick::GetInstance().GetMs();
                    this->mBmpSound.bmp = const_cast<uint8_t *>(Bitmaps::SoundLeft);
                    this->mSsd1306.DrawBitmap(
                            &this->mBmpSound, 0U, 12U, Bitmaps::Color::COLOR_WHITE);
                } else {
                    this->mNotifiedTimeSoundLeft = 0U;
                    this->mSsd1306.EraseArea(0U, 12U, this->mBmpSound.width, 8U);
                }
            } else {
                this->mNotifiedTimeSoundRight = 0U;
                this->mNotifiedTimeSoundLeft = 0U;
                this->mSsd1306.EraseArea(
                        (SCREEN_WIDTH) - (this->mBmpSound.width), 12U, this->mBmpSound.width, 8U);
                this->mSsd1306.EraseArea(0U, 12U, this->mBmpSound.width, 8U);
            }
        }
        
        void ServiceDisplay::DisplayGamepadState(const GamepadConnectedState &gamepadState){
            if (gamepadState.connected) {
                this->mSsd1306.DrawBitmap(&this->mBmpGamepad,
                                            SCREEN_WIDTH - this->mBmpCommunication.width - 2U -
                                              this->mBmpButton.width - 2U- this->mBmpGamepad.width,
                                            0U,
                                            Bitmaps::Color::COLOR_WHITE);
            } else {
                this->mSsd1306.EraseArea(SCREEN_WIDTH - this->mBmpCommunication.width - 2U -
                                              this->mBmpButton.width - 2U- this->mBmpGamepad.width,
                                            0U,
                                            this->mBmpGamepad.width,
                                            8U);
            }
        }

        void ServiceDisplay::OnEvent(const Event::Event &event) {
            if (event.serviceId == EServices::COMMUNICATION) {
                if (event.eventType == EventType::EVENT_COM_UPDATE) {
                    if (event.eventArg.type() == typeid(CommunicationStruct)) {
                        const CommunicationStruct comStruct =
                                std::any_cast<CommunicationStruct>(event.eventArg);
                        this->mState = comStruct;
                        if (this->mState == CLIENT_CONNECTED) {
                            this->mSsd1306.DrawBitmap(&this->mBmpCommunication,
                                                      SCREEN_WIDTH - this->mBmpCommunication.width,
                                                      0U,
                                                      Bitmaps::Color::COLOR_WHITE);
                        }
                    }
                }
            } else if (event.serviceId == EServices::BUTTON) {
                if (event.eventType == EventType::EVENT_BUTTON_UPDATE) {
                    if (event.eventArg.type() == typeid(Component::Button::ButtonStruct)) {
                        const Component::Button::ButtonStruct buttonStruct =
                                std::any_cast<Component::Button::ButtonStruct>(event.eventArg);
                        this->DisplayButtonBmp(buttonStruct.state);
                    }
                }
            } else if (event.serviceId == EServices::BATTERY) {
                if (event.eventType == EventType::EVENT_BATTERY_UPDATE) {
                    if (event.eventArg.type() == typeid(Component::Battery::BatteryStruct)) {
                        const Component::Battery::BatteryStruct batteryStruct =
                                std::any_cast<Component::Battery::BatteryStruct>(event.eventArg);
                        this->DisplayBatteryLevel(batteryStruct.state);
                    }
                }
            } else if (event.serviceId == EServices::PROXIMITY) {
                if (event.eventType == EventType::EVENT_SENSOR_UPDATE) {
                    if (event.eventArg.type() == typeid(Component::Proximity::SensorsStruct)) {
                        const Component::Proximity::SensorsStruct sensorStruct =
                                std::any_cast<Component::Proximity::SensorsStruct>(event.eventArg);
                        this->DisplayProximitySensor(sensorStruct.id, sensorStruct.distance);
                    }
                }
            } else if (event.serviceId == EServices::SOUND) {
                if (event.eventType == EventType::EVENT_SOUND_UPDATE) {
                    if (event.eventArg.type() == typeid(Component::Sound::SoundStruct)) {
                        const Component::Sound::SoundStruct soundStruct =
                                std::any_cast<Component::Sound::SoundStruct>(event.eventArg);
                        this->DisplaySound(soundStruct);
                    }
                }
            } else if (event.serviceId == EServices::GAMEPAD) {
                if (event.eventType == EventType::EVENT_GAMEPAD_UPDATE) {
                    if (event.eventArg.type() == typeid(Component::Gamepad::GamepadConnectedState)) {
                        const Component::Gamepad::GamepadConnectedState gamepadState =
                                std::any_cast<Component::Gamepad::GamepadConnectedState>(event.eventArg);
                        this->DisplayGamepadState(gamepadState);
                    }
                }
            }
        }
    } // namespace Display
} // namespace Service
