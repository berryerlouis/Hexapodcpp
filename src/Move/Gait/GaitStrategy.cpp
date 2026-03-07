#include "GaitStrategy.h"

namespace Move
{
    namespace Gait
    {
        void GaitTripod::doGaitStrategy(
                const bool                                               isCycleComplete,
                const uint64_t                                           currentTime,
                std::map<Bot::Legs::ELeg, Bot::Leg::Leg>                &legs,
                const Move::Gait::GaitParams                            &gaitParams,
                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                const uint8_t                                            stepPositionIndex,
                const float                                              normalizedTime) const {
            (void) isCycleComplete;
            for (auto &leg: legs) {
                uint8_t stepPositionIndexAlt = 0U;
                auto    legId = static_cast<uint8_t>(leg.second.GetId());
                if (legId % 2U == 0U) {
                    stepPositionIndexAlt = stepPositionIndex;
                } else {
                    stepPositionIndexAlt =
                            (stepPositionIndex + 1U) % static_cast<uint8_t>(positions.size());
                }
                Misc::Maths::Position3d position =
                        Misc::Utils::QuadraticLerp(positions[stepPositionIndexAlt][0U],
                                                   positions[stepPositionIndexAlt][1U],
                                                   positions[stepPositionIndexAlt][2U],
                                                   normalizedTime);
                leg.second.ComputeLerpTarget(currentTime,
                                             position,
                                             gaitParams.GetAmplitude(),
                                             gaitParams.GetElevation(),
                                             gaitParams.GetDirection(),
                                             gaitParams.IsRotated(),
                                             gaitParams.GetRotation(),
                                             gaitParams.GetRotationClockWize(),
                                             gaitParams.GetUpdatedTimeStamp());
                leg.second.SetTarget(position);
            }
        }

        void
        GaitWave::doGaitStrategy(const bool                                isCycleComplete,
                                 const uint64_t                            currentTime,
                                 std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                 const Move::Gait::GaitParams             &gaitParams,
                                 const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                                 const uint8_t stepPositionIndex,
                                 const float   normalizedTime) const {
            (void) isCycleComplete;
            (void) stepPositionIndex;
            float normalizedTimeAlt = 0U;
            float stepPositionIndexAlt = 0U;
            // WAVE: Only ONE leg moves at a time in
            // sequence (0->1->2->3->4->5->0...) Calculate
            // which leg should be moving based on the step
            // position

            for (auto &leg: legs) {
                auto legId = static_cast<uint8_t>(leg.second.GetId());
                bool move = false;
                if (!GaitWave::activeAllLegId) {
                    if ((legId == GaitWave::activeLegId) && (!isCycleComplete)) {
                        normalizedTimeAlt = normalizedTime;
                        stepPositionIndexAlt = 0U;
                        move = true;
                    }
                } else {
                    if (!isCycleComplete) {
                        normalizedTimeAlt = normalizedTime;
                        stepPositionIndexAlt = 1U;
                        move = true;
                    }
                }
                if (move) {
                    Misc::Maths::Position3d position =
                            Misc::Utils::QuadraticLerp(positions[stepPositionIndexAlt][0U],
                                                       positions[stepPositionIndexAlt][1U],
                                                       positions[stepPositionIndexAlt][2U],
                                                       normalizedTimeAlt);
                    leg.second.ComputeLerpTarget(currentTime,
                                                 position,
                                                 gaitParams.GetAmplitude(),
                                                 gaitParams.GetElevation(),
                                                 gaitParams.GetDirection(),
                                                 gaitParams.IsRotated(),
                                                 gaitParams.GetRotation(),
                                                 gaitParams.GetRotationClockWize(),
                                                 gaitParams.GetUpdatedTimeStamp());
                    leg.second.SetTarget(position);
                }
            }
            if (isCycleComplete) {
                if (!GaitWave::activeAllLegId) {
                    GaitWave::activeLegId = (GaitWave::activeLegId + 1U);
                    if (GaitWave::activeLegId >= static_cast<uint8_t>(legs.size())) {
                        GaitWave::activeAllLegId = true;
                        GaitWave::activeLegId = 0U;
                    }
                } else {
                    GaitWave::activeAllLegId = false;
                }
            }
        }

        void GaitRipple::doGaitStrategy(
                const bool                                               isCycleComplete,
                const uint64_t                                           currentTime,
                std::map<Bot::Legs::ELeg, Bot::Leg::Leg>                &legs,
                const Move::Gait::GaitParams                            &gaitParams,
                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                const uint8_t                                            stepPositionIndex,
                const float                                              normalizedTime) const {
            (void) isCycleComplete;
            for (auto &leg: legs) {
                uint8_t stepPositionIndexAlt = 0U;
                auto    legId = static_cast<uint8_t>(leg.second.GetId());
                if (legId % 2U == 0U) {
                    stepPositionIndexAlt = stepPositionIndex;
                } else {
                    stepPositionIndexAlt =
                            (stepPositionIndex + 1U) % static_cast<uint8_t>(positions.size());
                }
                Misc::Maths::Position3d position =
                        Misc::Utils::QuadraticLerp(positions[stepPositionIndexAlt][0U],
                                                   positions[stepPositionIndexAlt][1U],
                                                   positions[stepPositionIndexAlt][2U],
                                                   normalizedTime);
                leg.second.ComputeLerpTarget(currentTime,
                                             position,
                                             gaitParams.GetAmplitude(),
                                             gaitParams.GetElevation(),
                                             gaitParams.GetDirection(),
                                             gaitParams.IsRotated(),
                                             gaitParams.GetRotation(),
                                             gaitParams.GetRotationClockWize(),
                                             gaitParams.GetUpdatedTimeStamp());
                leg.second.SetTarget(position);
            }
        }
    } // namespace Gait
} // namespace Move
