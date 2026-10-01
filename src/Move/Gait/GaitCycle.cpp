#include "GaitCycle.h"

#include <cmath>

namespace Move
{
    namespace Gait
    {
        GaitCycle::GaitCycle(Bot::Legs::LegsInterface &legs, GaitParams &gaitParams)
            : mLegs(legs)
            , mGaitParams(gaitParams)
            , mGaitTripod()
            , mGaitWave()
            , mGaitRipple()
            , mGaitStrategy(&mGaitTripod)
            , mLastUpdateTime(0U)
            , mHasLastUpdateTime(false)
            , mPhase(0.0F)
            , mRamp(0.0F)
            , mResetPhaseWhenIdle(false)
            , mSettled(true) {
            this->mGaitParams.SetRunning(false);
        }

        bool GaitCycle::Start() {
            if (!this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(true);
                this->mHasLastUpdateTime = false;
                this->mResetPhaseWhenIdle = false;
                this->mSettled = false;
                return true;
            }
            return false;
        }

        bool GaitCycle::Pause() {
            if (this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(false);
                this->mSettled = false;
                return true;
            }
            return false;
        }

        bool GaitCycle::Stop() {
            this->mResetPhaseWhenIdle = true;
            if (this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(false);
                this->mSettled = false;
                return true;
            }
            return false;
        }

        bool GaitCycle::SetGaitType(GaitType gaitType) {
            if (this->mGaitParams.SetGaitType(gaitType)) {
                this->SelectStrategy(gaitType);
                // Legs are in different phases between gaits: restart from the neutral pose.
                this->mPhase = 0.0F;
                this->mRamp = 0.0F;
                return true;
            }
            return false;
        }

        void GaitCycle::SelectStrategy(const GaitType gaitType) {
            if (gaitType == GaitType::WAVE) {
                this->mGaitStrategy = &this->mGaitWave;
            } else if (gaitType == GaitType::RIPPLE) {
                this->mGaitStrategy = &this->mGaitRipple;
            } else {
                this->mGaitStrategy = &this->mGaitTripod;
            }
        }

        void GaitCycle::Update(const uint64_t currentTime) {
            uint64_t deltaMs = 0U;
            if (this->mHasLastUpdateTime && currentTime > this->mLastUpdateTime) {
                deltaMs = currentTime - this->mLastUpdateTime;
                if (deltaMs > MAX_STEP_MS) {
                    deltaMs = MAX_STEP_MS;
                }
            }
            this->mLastUpdateTime = currentTime;
            this->mHasLastUpdateTime = true;

            const bool  running = this->mGaitParams.IsRunning();
            const float rampStep = static_cast<float>(deltaMs) / static_cast<float>(RAMP_DURATION_MS);
            if (running) {
                this->mRamp = (this->mRamp + rampStep > 1.0F) ? 1.0F : this->mRamp + rampStep;

                // One trajectory segment lasts one cycle duration.
                const float duration = static_cast<float>(this->mGaitParams.GetCycleDuration());
                if (duration > 0.0F) {
                    this->mPhase = std::fmod(this->mPhase + (static_cast<float>(deltaMs) / duration),
                                             TRAJECTORY_SEGMENTS);
                }
            } else {
                this->mRamp = (this->mRamp - rampStep < 0.0F) ? 0.0F : this->mRamp - rampStep;
            }

            if (!running && this->mRamp <= 0.0F) {
                if (this->mResetPhaseWhenIdle) {
                    this->mPhase = 0.0F;
                    this->mResetPhaseWhenIdle = false;
                }
                // Neutral pose already sent, leave the legs to the posture commands.
                if (this->mSettled) {
                    return;
                }
                this->mSettled = true;
            } else {
                this->mSettled = false;
            }

            // Smoothstep avoids a velocity step at both ends of the blend.
            this->ApplyGait(this->mRamp * this->mRamp * (3.0F - (2.0F * this->mRamp)));
            this->mLegs.Update();
        }

        void GaitCycle::ApplyGait(const float scale) {
            this->mGaitStrategy->doGaitStrategy(
                    this->mLegs.GetLegs(), this->mGaitParams, this->mTrajectory, this->mPhase, scale);
        }
    } // namespace Gait
} // namespace Move
