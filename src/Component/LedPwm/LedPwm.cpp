#include "LedPwm.h"

namespace Component
{
    namespace LedPwm
    {
        LedPwm::LedPwm(Led::LedInterface &led) :
            mLed(led)
            , mDutyCycle(50U)
            , mLastTime(0U)
            , mIndexInterval(0U)
            , mInterval{100, 100, 200, 500}
            , mSpeedInterval(2U)
            , mToggleFade(false)
            , mRunning(false)
            , mFadeDuration(0)
            , mFadeStep(0) {
            LOG_COMPONENT_DEBUG("Led PWM", "Initialized.");
        }

        LedPwm::~LedPwm() {
            this->Stop();
        }

        Core::Status LedPwm::Initialize(void) {
            this->mRunning = true;
            this->mPwmThread = std::thread(&LedPwm::PwmControl, this);
            return Core::CORE_OK;
        }


        void LedPwm::Update(const uint64_t currentTime) {
            if (currentTime > this->mLastTime + this->mInterval[this->mIndexInterval] * this->mSpeedInterval) {
                this->mLastTime = currentTime;
                if (this->mToggleFade == true) {
                    this->mToggleFade = false;
                    this->FadeIn(this->mInterval[this->mIndexInterval] * this->mSpeedInterval);
                } else {
                    this->mToggleFade = true;
                    this->FadeOut(this->mInterval[this->mIndexInterval] * this->mSpeedInterval);
                }
                this->mIndexInterval++;
                if (this->mIndexInterval == NB_INTERVAL) {
                    this->mIndexInterval = 0U;
                }
            }
        }

        void LedPwm::Stop() {
            this->mRunning = false;
            if (this->mPwmThread.joinable()) {
                this->mPwmThread.join();
            }
        }

        void LedPwm::SetDutyCycle(const uint16_t duty) {
            this->mDutyCycle = duty;
        }

        void LedPwm::FadeIn(const uint16_t duration) {
            this->mFadeDuration = duration;
            this->mFadeStep = 1;
        }

        void LedPwm::FadeOut(const uint16_t duration) {
            this->mFadeDuration = duration;
            this->mFadeStep = -1;
        }

        void LedPwm::PwmControl() {
            const auto startTime = std::chrono::steady_clock::now();
            while (this->mRunning) {
                auto currentTime = std::chrono::steady_clock::now();
                const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(
                        currentTime - startTime).count();

                if (this->mFadeDuration > 0) {
                    const uint16_t steps = this->mFadeDuration / 100U;
                    if (elapsedTime % steps == 0U) {
                        const uint16_t newDutyCycle = this->mDutyCycle + this->mFadeStep;
                        if (newDutyCycle <= 100U) {
                            this->mDutyCycle = newDutyCycle;
                        } else {
                            this->mFadeDuration = 0U;
                        }
                    }
                }

                const int onTime = this->mDutyCycle * 10U; // Assuming 1000us period
                const int offTime = 1000U - onTime;

                auto pwmStartTime = std::chrono::steady_clock::now();
                while (std::chrono::duration_cast<std::chrono::microseconds>(
                               std::chrono::steady_clock::now() - pwmStartTime).count() < onTime) {
                    this->mLed.On();
                    std::this_thread::sleep_for(std::chrono::microseconds(5U));
                }

                pwmStartTime = std::chrono::steady_clock::now();
                while (std::chrono::duration_cast<std::chrono::microseconds>(
                               std::chrono::steady_clock::now() - pwmStartTime).count() < offTime) {
                    this->mLed.Off();
                    std::this_thread::sleep_for(std::chrono::microseconds(5U));
                }
            }
        }
    }
}
