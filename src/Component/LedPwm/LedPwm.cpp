#include "LedPwm.h"
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
            , mFrequency(1.2F) // ~72 BPM resting heartbeat
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

            // Advance the heartbeat waveform every mFadeStepMs for a smooth animation.
            if (currentTime >= this->mLastUpdate + this->mFadeStepMs) {
                this->UpdateHeartbeat();
                this->mLastUpdate = currentTime;
            }

            // Apply current duty cycle to LED
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

        void LedPwm::UpdateHeartbeat() {
            // mFrequency is interpreted as beats per second (Hz). One full
            // heartbeat cycle is a normalized phase in [0, 1) composed of:
            //   * a primary "lub" pulse (full amplitude)
            //   * a short gap
            //   * a smaller "dub" pulse (~70% amplitude)
            //   * a longer rest until the next beat
            // Each pulse uses a half-sine shape for a smooth, organic feel.
            const float timeStep = static_cast<float>(this->mFadeStepMs) / 1000.0F;
            this->mPhase += this->mFrequency * timeStep;
            if (this->mPhase >= 1.0F) {
                this->mPhase -= floorf(this->mPhase);
            }

            // Pulse window boundaries inside the normalized [0, 1) cycle.
            constexpr float LUB_START = 0.00F;
            constexpr float LUB_END = 0.18F;
            constexpr float DUB_START = 0.28F;
            constexpr float DUB_END = 0.42F;
            constexpr float DUB_AMPLITUDE_RATIO = 0.70F;

            float intensity = 0.0F; // 0..1
            if (this->mPhase >= LUB_START && this->mPhase < LUB_END) {
                const float local = (this->mPhase - LUB_START) / (LUB_END - LUB_START);
                intensity = sinf(local * static_cast<float>(M_PI));
            } else if (this->mPhase >= DUB_START && this->mPhase < DUB_END) {
                const float local = (this->mPhase - DUB_START) / (DUB_END - DUB_START);
                intensity = sinf(local * static_cast<float>(M_PI)) * DUB_AMPLITUDE_RATIO;
            }

            if (intensity < 0.0F) {
                intensity = 0.0F;
            } else if (intensity > 1.0F) {
                intensity = 1.0F;
            }

            this->mDutyCycle =
                    static_cast<uint16_t>(intensity * static_cast<float>(this->mAmplitude));
        }

    } // namespace LedPwm
} // namespace Component
