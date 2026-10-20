#ifndef LED_PWM_H
#define LED_PWM_H

#include <cstdint>
#include "../../Core/Status.h"
#include "../Led/LedInterface.h"
#include "LedPwmInterface.h"

namespace Component
{
    namespace LedPwm
    {

        class LedPwm : public LedPwmInterface {
        public:
            explicit LedPwm(Led::LedInterface &led);

            ~LedPwm();

            virtual Core::Status Initialize(void) final override;

            virtual void         Update(const uint64_t currentTime) final override;

            virtual void         Stop(void) final override;

            virtual void         UpdateFrequency(const float frequency) final override;


        private:
            void               UpdateHeartbeat(void);
            Led::LedInterface &mLed;

            uint16_t           mDutyCycle;
            float              mPhase;
            uint64_t           mLastUpdate;
            bool               mRunning;
            uint16_t           mFadeStepMs;
            float              mFrequency;
            uint16_t           mAmplitude;
        };

    } // namespace LedPwm
} // namespace Component

#endif // LED_PWM_H
