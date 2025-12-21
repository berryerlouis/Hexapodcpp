#include "GaitCycle.h"
#include "../../Core/Logger.h"
#include "../../Driver/Timer/Tick.h"

namespace Move
{
    namespace Gait
    {
        GaitCycle::GaitCycle(Bot::Legs::LegsInterface &legs,
                             GaitParams               &gaitParams) :
            mLegs(legs),
            mGaitParams(gaitParams),
            mLastUpdateTime(0UL),
            mGaitStrategy(std::move(std::make_unique<Move::Gait::GaitTripod>())),
            mStartTime(0UL),
            mStepPositionIndex(0U) {
            this->mGaitParams.SetRunning(true);
        }

        bool GaitCycle::Start(void) {
            if (!this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(true);
                this->mStartTime = Driver::Timer::Tick::GetInstance().GetMs();
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

        bool GaitCycle::Stop(void) {
            if (this->mGaitParams.IsRunning()) {
                this->mGaitParams.SetRunning(false);
                this->mStartTime = Driver::Timer::Tick::GetInstance().GetMs();
                return true;
            }
            return false;
        }

        bool GaitCycle::SetGaitType(GaitType gaitType) {
            if (this->mGaitParams.SetGaitType(gaitType)) {
                if (this->mGaitParams.GetGaitType() == Move::Gait::GaitType::TRIPOD) {
                    this->mGaitStrategy = std::move(std::make_unique<Move::Gait::GaitTripod>());
                } else if (this->mGaitParams.GetGaitType() == Move::Gait::GaitType::WAVE) {
                    this->mGaitStrategy = std::move(std::make_unique<Move::Gait::GaitWave>());
                } else {
                    this->mGaitStrategy = std::move(std::make_unique<Move::Gait::GaitRipple>());
                }
                return true;
            }
            return false;
        }

        void GaitCycle::Update(const uint64_t currentTime) {
            if (!this->mGaitParams.IsRunning()) {
                bool isCycleComplete = this->IsCycleComplete(currentTime);
                if (!isCycleComplete) {
                    for (auto &leg: this->mLegs.GetLegs()) {
                        Misc::Maths::Position3d position = this->mPositionsStop;
                        leg.second.ComputeLerpTarget(currentTime,
                                                     position,
                                                     this->mGaitParams.GetAmplitude(),
                                                     this->mGaitParams.GetElevation(),
                                                     this->mGaitParams.GetDirection(),
                                                     this->mGaitParams.IsRotated(),
                                                     this->mGaitParams.GetRotation(),
                                                     this->mGaitParams.GetRotationClockWize(),
                                                     this->mGaitParams.GetUpdatedTimeStamp());
                        leg.second.SetTarget(position);
                    }
                }
            } else {
                bool isCycleComplete = this->IsCycleComplete(currentTime);
                if (isCycleComplete) {
                    this->AdvanceToNextCycle(currentTime);
                }

                const float normalizedTime = this->GetNormalizedTime(currentTime);

                mGaitStrategy->doGaitStrategy(isCycleComplete,
                                              currentTime,
                                              this->mLegs.GetLegs(),
                                              this->mGaitParams,
                                              this->mPositions,
                                              this->mStepPositionIndex,
                                              normalizedTime);
            }
            this->mLegs.Update();
        }

        float GaitCycle::GetDeltaTimeOfCycleDuration(const uint64_t currentTime) const {
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
    } // namespace Gait
} // namespace Move
