#include "LedPwm.h"
#include <algorithm>
#include <cmath>

namespace Component
{
    namespace LedPwm
    {
        LedPwm::LedPwm(Led::LedInterface &led)
            : mLed(led)
            , mDutyCycle(0U)
            , mPhase(0.0F)
            , mLastUpdate(0U)
            , mRunning(false)
            , mFadeStepMs(5U)
            , mFrequency(2.0F)
            , mAmplitude(1000U) {
        }

        LedPwm::~LedPwm() {
            this->Stop();
        }

        Core::Status LedPwm::Initialize() {
            this->mRunning = true;
            this->mLastUpdate = 0U;
            this->mDutyCycle = 0U;
            this->mPhase = 0.0F;
            LOG_COMPONENT_DEBUG("LedPwm", "Initialized.");
            return Core::CORE_OK;
        }

        void LedPwm::Update(const uint64_t currentTime) {
            if (!this->mRunning) {
                return;
            }

            // Update sine wave every 5ms for smooth
            // animation
            if (currentTime >= this->mLastUpdate + this->mFadeStepMs) {
                this->UpdateSineWave();
                this->mLastUpdate = currentTime;
            }

            // Apply current sine duty cycle to LED
            this->mLed.Pwm(this->mDutyCycle);
        }

        void LedPwm::Stop() {
            this->mRunning = false;
            this->mDutyCycle = 0U;
            this->mLed.Off();
        }

        void LedPwm::UpdateFrequency(const float frequency) {
            this->mFrequency = frequency;
            this->mPhase = 0.0F;
        }

        void LedPwm::UpdateSineWave() {
            // Phase advance: angular frequency = 2πf, time
            // step = 5ms
            const float timeStep = 0.005f; // 5ms in seconds
            this->mPhase += 2.0F * M_PI * this->mFrequency * timeStep;

            // Wrap phase to [0, 2π]
            if (this->mPhase >= M_PI * 2.0F) {
                this->mPhase -= M_PI * 2.0F;
            }

            // Compute sine wave: sin(φ) ∈ [-1,1] → [0,1000]
            // scaled by amplitude
            const float    sineValue = sinf(this->mPhase);
            const uint16_t targetDuty =
                    static_cast<uint16_t>((sineValue + 1.0F) * 0.5f * this->mAmplitude);
            this->mDutyCycle = targetDuty;
            // Smooth transition to prevent flicker
            /*if (this->mDutyCycle < targetDuty) {
                this->mDutyCycle = std::min(targetDuty,
            static_cast<uint16_t>(this->mDutyCycle + 20U));
            } else if (this->mDutyCycle > targetDuty) {
            this->mDutyCycle = std::max(targetDuty,
            static_cast<uint16_t>(this->mDutyCycle - 20U));
            }*/
        }

    } // namespace LedPwm
} // namespace Component
