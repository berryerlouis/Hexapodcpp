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
        using namespace Component::Display;
        using namespace Component::Proximity;
        using namespace Component::Sound;
        using namespace Component::Communication;
        using namespace Misc::Bitmap;

        using namespace Component;

        class ServiceDisplay : public Service, Event::EventListenerInterface {
        public:
            ServiceDisplay(Ssd1306Interface                &ssd1306,
                           Message::MessageInterface       &messageListener,
                           Event::EventDispatcherInterface &eventDispatcher);

            ~ServiceDisplay() = default;

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         OnEvent(const Event::Event &event) final override;

            void                 DisplayBackground(void) const;

            void                 DisplayCommunicationBmp(void);

            void                 DisplayButtonBmp(const Button::ButtonState &buttonState);

            void                 DisplayBatteryLevel(const Battery::BatteryState state);

            void                 DisplayProximitySensor(const Component::Proximity::SensorsId sensorId,
                                                        const uint16_t                        distance);

            void                 DisplaySound(const SoundStruct &soundStruct);

        private:
            Ssd1306Interface   &mSsd1306;
            Bitmaps::SBitmap    mBmpBatteryLevel;
            Bitmaps::SBitmap    mBmpCommunication;
            Bitmaps::SBitmap    mBmpProximity;
            Bitmaps::SBitmap    mBmpButton;
            Bitmaps::SBitmap    mBmpSound;
            uint64_t            mPreviousTime;
            uint32_t            mToggleCommunicationBmp;
            CommunicationStruct mState;
            uint64_t            mNotifiedTimeProximityUsLeft;
            uint64_t            mNotifiedTimeProximityUsRight;
            uint64_t            mNotifiedTimeProximityLaser;
            uint64_t            mNotifiedTimeSoundLeft;
            uint64_t            mNotifiedTimeSoundRight;
        };
    } // namespace Display
} // namespace Service
