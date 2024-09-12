#pragma once

#include "../../Component/Display/Ssd1306Interface.h"
#include "../../Component/Battery/BatteryState.h"
#include "../../Component/Proximity/SensorsId.h"
#include "../../Misc/Bitmap/Bitmaps.h"
#include "../Service.h"

namespace Service
{
    namespace Display
    {
        using namespace Component::Battery;
        using namespace Component::Proximity;
        using namespace Component::Display;
        using namespace Misc::Bitmap;

        using namespace Component;

        class ServiceDisplay : public Service {
        public:
            ServiceDisplay(Ssd1306Interface &ssd1306, Event::EventListener &eventListener);

            ~ServiceDisplay() = default;

            virtual Core::CoreStatus Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void DispatchEvent(SEvent &event) final override;

            void DisplayBackground(void) const;

            void DisplayCommunicationBmp(void);

            void DisplayBatteryLevel(const Battery::BatteryState state);

            void DisplayProximitySensor(const Component::Proximity::SensorsId sensorId, const uint16_t distance);

        private:
            Ssd1306Interface &mSsd1306;
            Bitmaps::SBitmap mBmpBatteryLevel;
            Bitmaps::SBitmap mBmpCommunication;
            Bitmaps::SBitmap mBmpProximity;
            uint64_t mPreviousTime;
            uint32_t mToggleCommunicationBmp;
        };
    }
}
