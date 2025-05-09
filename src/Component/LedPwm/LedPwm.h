#pragma once

#include "LedPwmInterface.h"
#include "../Led/LedInterface.h"
#include <thread>
#include <chrono>
#include <atomic>


namespace Component
{
    namespace LedPwm
    {
        class LedPwm : public LedPwmInterface {
        public:
            LedPwm(Led::LedInterface &led);

            ~LedPwm();

            virtual Core::Status Initialize(void) final override;

            virtual void Update(const uint64_t currentTime) final override;

            virtual void Stop() final override;

            virtual void SetDutyCycle(const uint16_t duty) final override;

            virtual void FadeIn(const uint16_t duration) final override;

            virtual void FadeOut(const uint16_t duration) final override;

        private:
#define NB_INTERVAL  4U
            Led::LedInterface &mLed;
            std::atomic<uint16_t> mDutyCycle;

            uint64_t mLastTime;
            uint8_t mIndexInterval;
            uint64_t mInterval[NB_INTERVAL];
            uint8_t mSpeedInterval;
            bool mToggleFade;
            std::atomic<bool> mRunning;
            std::thread mPwmThread;
            std::atomic<int> mFadeDuration;
            std::atomic<int> mFadeStep;

            void PwmControl();
        };
    }
}
