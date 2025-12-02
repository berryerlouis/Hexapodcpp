#include "GaitCycle.h"
#include "../../Core/Logger.h"
#include "../../Driver/Tick/Tick.h"

namespace Move
{
    namespace Gait
    {        
        GaitCycle::GaitCycle(Bot::Legs::LegsInterface &legs, GaitParams &gaitParams)
            : mLegs(legs)
            , mGaitParams(gaitParams)
            , mLastUpdateTime(0UL)
            , mStartTime(0UL)
            , mStepPositionIndex(0U) {
                this->mGaitParams.SetRunning(true);
        }

        bool GaitCycle::Start(void) {
            if (!this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(true);
                this->mStartTime = Driver::Tick::Tick::GetInstance().GetMs();
                return true;
            }
            return false;
        }

        bool GaitCycle::Pause(void) const {
            if (this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(false);
                return true;
            }
            return false;
        }

        bool GaitCycle::Stop(void) const{
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

            float normalizedTime = this->GetNormalizedTime(currentTime);
            this->mLegs.ComputeTarget(currentTime, 
                                      this->mGaitParams,
                                      this->mPositions,
                                      this->mStepPositionIndex,
                                      normalizedTime);
            
            if (this->IsCycleComplete(currentTime)) {
                this->AdvanceToNextCycle(currentTime);
                this->mLegs.ComputeTarget(currentTime, 
                                      this->mGaitParams,
                                      this->mPositions,
                                      this->mStepPositionIndex,
                                      normalizedTime);
            }
            
            this->mLegs.Update();
        }

        float GaitCycle::GetDeltaTimeOfCycleDuration(const uint64_t currentTime) const{
            return this->GetNormalizedTime(currentTime);
        }

        float GaitCycle::GetNormalizedTime(const uint64_t currentTime) const {
            float deltaTimeMs = static_cast<float>(currentTime - this->mStartTime);
            float cycleDurationMs = this->mGaitParams.GetCycleDuration();
            float normalizedTime = deltaTimeMs / cycleDurationMs;
            return (normalizedTime > 1.0F) ? 1.0F : normalizedTime;
        }

        bool GaitCycle::IsCycleComplete(const uint64_t currentTime) const {
            float deltaTimeMs = static_cast<float>(currentTime - this->mStartTime);
            float cycleDurationMs = this->mGaitParams.GetCycleDuration();
            return deltaTimeMs >= cycleDurationMs;
        }

        void GaitCycle::AdvanceToNextCycle(const uint64_t currentTime) {
            this->mStartTime = currentTime;
            this->mStepPositionIndex = (this->mStepPositionIndex + 1U) % this->mPositions.size();
        }
    }
}
