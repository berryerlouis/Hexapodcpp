#pragma once


#include "../../Component/Display/Ssd1306Interface.h"
#include "../../Component/Battery/BatteryState.h"
#include "../../Component/Button/ButtonInterface.h"
#include "../../Component/Button/ButtonObserverInterface.h"
#include "../../Component/Proximity/SensorsId.h"
#include "../../Component/Proximity/SensorProximityInterface.h"
#include "../../Component/Proximity/SensorProximityObserverInterface.h"
#include "../../Component/Sound/SoundInterface.h"
#include "../../Component/Sound/SoundObserverInterface.h"
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
        using namespace Misc::Bitmap;

        using namespace Component;

        class ServiceDisplay : public Service
                               , private ButtonObserverInterface
                               , private SoundObserverInterface
                               , private SensorProximityObserverInterface {
        public:
            ServiceDisplay(Ssd1306Interface &ssd1306
                           , ButtonInterface &button
                           , SoundInterface &soundInterfaceLeft
                           , SoundInterface &soundInterfaceRight
                           , SensorProximityMultipleInterface &sensors
                           , Event::MessageInterface &messageListener);

            ~ServiceDisplay() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void UpdatedButtonState(const ButtonState &buttonState, const uint16_t period) final override;

            virtual void Detect(const SensorsId &sensorId, const uint16_t distance) final override;

            virtual void UpdatedSoundState(const SoundId &soundId, const SoundState &soundState,
                                           const uint16_t period) final override;

            void DisplayBackground(void) const;

            void DisplayCommunicationBmp(void);

            void DisplayButtonBmp(const ButtonState &buttonState);

            void DisplayBatteryLevel(const Battery::BatteryState state);

            void DisplayProximitySensor(const Component::Proximity::SensorsId sensorId, const uint16_t distance);

            void DisplaySound(const SoundId &soundId, const SoundState &soundState, const uint16_t period);

        private:
            Ssd1306Interface &mSsd1306;
            ButtonInterface &mButton;
            SoundInterface &mSoundInterfaceLeft;
            SoundInterface &mSoundInterfaceRight;
            SensorProximityMultipleInterface &mSensors;
            Bitmaps::SBitmap mBmpBatteryLevel;
            Bitmaps::SBitmap mBmpCommunication;
            Bitmaps::SBitmap mBmpProximity;
            Bitmaps::SBitmap mBmpButton;
            Bitmaps::SBitmap mBmpSound;
            uint64_t mPreviousTime;
            uint32_t mToggleCommunicationBmp;
        };
    }
}
