#pragma once


#include "../../Component/Battery/BatteryInterface.h"
#include "../../Component/Button/ButtonInterface.h"
#include "../../Component/Communication/CommunicationInterface.h"
#include "../../Component/Display/Ssd1306Interface.h"
#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../../Component/Proximity/SensorsId.h"
#include "../../Component/Sound/SoundInterface.h"
#include "../../Core/ObserverInterface.h"
#include "../../Misc/Bitmap/Bitmaps.h"
#include "../Service.h"

namespace Service
{
    namespace Display
    {
        using namespace Component::Battery;
        using namespace Component::Button;
        using namespace Component::Display;
        using namespace Component::Proximity;
        using namespace Component::Sound;
        using namespace Component::Communication;
        using namespace Misc::Bitmap;

        using namespace Component;

        class ServiceDisplay : public Service,
                               Core::ObserverInterface<BatteryStruct>,
                               Core::ObserverInterface<ButtonStruct>,
                               Core::ObserverInterface<SoundStruct>,
                               Core::ObserverInterface<SensorsStruct> {
        public:
            ServiceDisplay(Ssd1306Interface                 &ssd1306,
                           BatteryInterface                 &battery,
                           ButtonInterface                  &button,
                           SoundInterface                   &soundInterfaceLeft,
                           SoundInterface                   &soundInterfaceRight,
                           SensorProximityMultipleInterface &sensors,
                           Message::MessageInterface        &messageListener,
                           Event::EventListenerInterface    &eventListener);

            ~ServiceDisplay() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         DispatchEvent(const Event::Event &event) final override;

            virtual void         Notified(const ButtonStruct &button) final override;

            virtual void         Notified(const SoundStruct &sound) final override;

            virtual void         Notified(const SensorsStruct &sensor) final override;

            virtual void         Notified(const BatteryStruct &state) final override;

            void                 DisplayBackground(void) const;

            void                 DisplayCommunicationBmp(void);

            void                 DisplayButtonBmp(const ButtonState &buttonState);

            void                 DisplayBatteryLevel(const Battery::BatteryState state);

            void                 DisplayProximitySensor(const Component::Proximity::SensorsId sensorId,
                                                        const uint16_t                        distance);

            void                 DisplaySound(const SoundStruct &soundStruct);

        private:
            Ssd1306Interface                 &mSsd1306;
            BatteryInterface                 &mBattery;
            ButtonInterface                  &mButton;
            SoundInterface                   &mSoundLeft;
            SoundInterface                   &mSoundRight;
            SensorProximityMultipleInterface &mSensors;
            Bitmaps::SBitmap                  mBmpBatteryLevel;
            Bitmaps::SBitmap                  mBmpCommunication;
            Bitmaps::SBitmap                  mBmpProximity;
            Bitmaps::SBitmap                  mBmpButton;
            Bitmaps::SBitmap                  mBmpSound;
            uint64_t                          mPreviousTime;
            uint32_t                          mToggleCommunicationBmp;
            CommunicationStruct               mState;
            uint64_t                          mNotifiedTimeProximityUsLeft;
            uint64_t                          mNotifiedTimeProximityUsRight;
            uint64_t                          mNotifiedTimeProximityLaser;
            uint64_t                          mNotifiedTimeSoundLeft;
            uint64_t                          mNotifiedTimeSoundRight;
        };
    } // namespace Display
} // namespace Service
