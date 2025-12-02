#include "GaitCycle.h"
#include "../../Driver/Tick/Tick.h"

namespace Move
{
    namespace Gait
    {
        GaitCycle::GaitCycle(Bot::Legs::LegsInterface &legs, GaitParams &gaitParams) :
            mLegs(legs)
            , mGaitParams(gaitParams)
            , mLastUpdateTime(0UL)
            , mStartTime(0UL)
            , mStepPositionIndex(0U) {
            this->mGaitParams.SetRunning(true);
        }

        bool GaitCycle::Start() {
            if (!this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(true);
                this->mStartTime = Driver::Tick::Tick::GetInstance().GetMs();
                return true;
            }
            return false;
        }

        bool GaitCycle::Pause() const {
            if (this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(false);
                return true;
            }
            return false;
        }

        bool GaitCycle::Stop() const {
            if (this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(false);
                this->mLegs.ResetLegs(this->mGaitParams.GetCycleDuration());
                return true;
            }
            return false;
        }

        void GaitCycle::Update(const uint64_t currentTime) {
            if (!this->mGaitParams.IsRunning()) {
                return;
            }

            const float normalizedTime = this->GetNormalizedTime(currentTime);
            this->mLegs.SetTarget(this->mGaitParams,
                                  this->mPositions,
                                  this->mStepPositionIndex,
                                  normalizedTime);

            if (this->IsCycleComplete(currentTime)) {
                this->AdvanceToNextCycle(currentTime);
                this->mLegs.SetTarget(this->mGaitParams,
                                      this->mPositions,
                                      this->mStepPositionIndex,
                                      0.0F);
            }

            this->mLegs.Update();
        }

        float GaitCycle::GetDeltaTimeOfCycleDuration(const uint64_t currentTime) const {
            return this->GetNormalizedTime(currentTime);
        }

        float GaitCycle::GetNormalizedTime(const uint64_t currentTime) const {
            const float deltaTimeMs = static_cast<float>(currentTime - this->mStartTime);
            const float cycleDurationMs = this->mGaitParams.GetCycleDuration();
            const float normalizedTime = deltaTimeMs / cycleDurationMs;
            return (normalizedTime > 1.0F) ? 1.0F : normalizedTime;
        }

        bool GaitCycle::IsCycleComplete(const uint64_t currentTime) const {
            const float deltaTimeMs = static_cast<float>(currentTime - this->mStartTime);
            const float cycleDurationMs = this->mGaitParams.GetCycleDuration();
            return deltaTimeMs >= cycleDurationMs;
        }

        void GaitCycle::AdvanceToNextCycle(const uint64_t currentTime) {
            this->mStartTime = currentTime;
            this->mStepPositionIndex = (this->mStepPositionIndex + 1U) % this->mPositions.size();
        }
    }
}
