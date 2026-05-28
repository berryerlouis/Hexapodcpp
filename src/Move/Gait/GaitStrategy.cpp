#include "GaitStrategy.h"

#include <cmath>

namespace
{
    // Maps any (possibly negative or very large) phase into [0, cycleLength).
    // Using fmodf rather than a while-loop keeps the cost bounded if phase
    // drifts unexpectedly (e.g. after a long pause / re-sync).
    float WrapPhase(const float phase, const float cycleLength) {
        if (cycleLength <= 0.0F) {
            return 0.0F;
        }
        float wrappedPhase = std::fmod(phase, cycleLength);
        if (wrappedPhase < 0.0F) {
            wrappedPhase += cycleLength;
        }
        return wrappedPhase;
    }

    uint8_t GetRipplePhaseRank(const Bot::Legs::ELeg legId) {
        switch (legId) {
            case Bot::Legs::FRONT_LEFT:
                return 0U;
            case Bot::Legs::MIDDLE_LEFT:
                return 1U;
            case Bot::Legs::REAR_LEFT:
                return 2U;
            case Bot::Legs::REAR_RIGHT:
                return 3U;
            case Bot::Legs::MIDDLE_RIGHT:
                return 4U;
            case Bot::Legs::FRONT_RIGHT:
                return 5U;
            default:
                return 0U;
        }
    }

    uint8_t GetWavePhaseRank(const Bot::Legs::ELeg legId) {
        switch (legId) {
            case Bot::Legs::FRONT_LEFT:
                return 0U;
            case Bot::Legs::FRONT_RIGHT:
                return 1U;
            case Bot::Legs::MIDDLE_LEFT:
                return 2U;
            case Bot::Legs::MIDDLE_RIGHT:
                return 3U;
            case Bot::Legs::REAR_LEFT:
                return 4U;
            case Bot::Legs::REAR_RIGHT:
                return 5U;
            default:
                return 0U;
        }
    }

    void ApplyLegTarget(const uint64_t                currentTime,
                        const uint64_t                transitionStartTime,
                        Bot::Leg::Leg                &leg,
                        const Move::Gait::GaitParams &gaitParams,
                        const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                        const uint8_t                                            stepPositionIndex,
                        const float                                              normalizedTime) {
        const uint8_t boundedStepPositionIndex =
                stepPositionIndex % static_cast<uint8_t>(positions.size());
        Misc::Maths::Position3d position =
                Misc::Utils::QuadraticLerp(positions[boundedStepPositionIndex][0U],
                                           positions[boundedStepPositionIndex][1U],
                                           positions[boundedStepPositionIndex][2U],
                                           normalizedTime);
        leg.ComputeLerpTarget(currentTime,
                              position,
                              gaitParams.GetAmplitude(),
                              gaitParams.GetElevation(),
                              gaitParams.GetDirection(),
                              gaitParams.IsRotated(),
                              gaitParams.GetRotation(),
                              gaitParams.GetRotationClockWize(),
                              transitionStartTime);
        leg.SetTarget(position);
    }

    void ApplyLegTargetWithDuty(const uint64_t                currentTime,
                                const uint64_t                transitionStartTime,
                                Bot::Leg::Leg                &leg,
                                const Move::Gait::GaitParams &gaitParams,
                                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                                const float                                              legPhase,
                                const float swingDuty) {
        if (positions.size() < 2U) {
            return;
        }

        const float cycleLength = static_cast<float>(positions.size());
        const float wrappedPhase = WrapPhase(legPhase, cycleLength);

        float       boundedSwingDuty = swingDuty;
        if (boundedSwingDuty < 0.01F) {
            boundedSwingDuty = 0.01F;
        }
        if (boundedSwingDuty > 0.99F) {
            boundedSwingDuty = 0.99F;
        }

        const float swingSpan = cycleLength * boundedSwingDuty;
        const float stanceSpan = cycleLength - swingSpan;

        if (wrappedPhase < swingSpan) {
            const float swingTime = wrappedPhase / swingSpan;
            ApplyLegTarget(
                    currentTime, transitionStartTime, leg, gaitParams, positions, 0U, swingTime);
        } else {
            const float stanceTime = (wrappedPhase - swingSpan) / stanceSpan;
            ApplyLegTarget(
                    currentTime, transitionStartTime, leg, gaitParams, positions, 1U, stanceTime);
        }
    }
} // namespace

namespace Move
{
    namespace Gait
    {
        void GaitTripod::doGaitStrategy(
                const bool                                               isCycleComplete,
                const uint64_t                                           currentTime,
                const uint64_t                                           transitionStartTime,
                std::map<Bot::Legs::ELeg, Bot::Leg::Leg>                &legs,
                const Move::Gait::GaitParams                            &gaitParams,
                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                const uint8_t                                            stepPositionIndex,
                const float                                              normalizedTime) {
            (void) isCycleComplete;

            const float globalPhase = static_cast<float>(stepPositionIndex) + normalizedTime;
            for (auto &leg: legs) {
                const uint8_t legId = static_cast<uint8_t>(leg.second.GetId());
                const float   phaseOffset = ((legId % 2U) == 0U) ? 0.0F : 1.0F;
                ApplyLegTargetWithDuty(currentTime,
                                       transitionStartTime,
                                       leg.second,
                                       gaitParams,
                                       positions,
                                       globalPhase + phaseOffset,
                                       0.5F);
            }
        }

        void
        GaitWave::doGaitStrategy(const bool                                isCycleComplete,
                                 const uint64_t                            currentTime,
                                 const uint64_t                            transitionStartTime,
                                 std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                 const Move::Gait::GaitParams             &gaitParams,
                                 const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                                 const uint8_t stepPositionIndex,
                                 const float   normalizedTime) {
            (void) isCycleComplete;

            const float globalPhase = static_cast<float>(stepPositionIndex) + normalizedTime;
            const float offsetPerLeg =
                    static_cast<float>(positions.size()) / static_cast<float>(Bot::Legs::NB_LEGS);

            // Wave gait: exactly one leg in swing at any time → swing duty = 1/N.
            const float swingDuty = 1.0F / static_cast<float>(Bot::Legs::NB_LEGS);

            for (auto &leg: legs) {
                const uint8_t phaseRank = GetWavePhaseRank(leg.second.GetId());
                const float   phaseOffset = offsetPerLeg * static_cast<float>(phaseRank);
                ApplyLegTargetWithDuty(currentTime,
                                       transitionStartTime,
                                       leg.second,
                                       gaitParams,
                                       positions,
                                       globalPhase + phaseOffset,
                                       swingDuty);
            }
        }

        void GaitRipple::doGaitStrategy(
                const bool                                               isCycleComplete,
                const uint64_t                                           currentTime,
                const uint64_t                                           transitionStartTime,
                std::map<Bot::Legs::ELeg, Bot::Leg::Leg>                &legs,
                const Move::Gait::GaitParams                            &gaitParams,
                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                const uint8_t                                            stepPositionIndex,
                const float                                              normalizedTime) {
            (void) isCycleComplete;

            const float globalPhase = static_cast<float>(stepPositionIndex) + normalizedTime;
            const float offsetPerLeg =
                    static_cast<float>(positions.size()) / static_cast<float>(Bot::Legs::NB_LEGS);

            // Ripple gait: two legs in swing simultaneously → swing duty = 2/N.
            const float swingDuty = 2.0F / static_cast<float>(Bot::Legs::NB_LEGS);

            for (auto &leg: legs) {
                const uint8_t phaseRank = GetRipplePhaseRank(leg.second.GetId());
                const float   phaseOffset = offsetPerLeg * static_cast<float>(phaseRank);
                ApplyLegTargetWithDuty(currentTime,
                                       transitionStartTime,
                                       leg.second,
                                       gaitParams,
                                       positions,
                                       globalPhase + phaseOffset,
                                       swingDuty);
            }
        }
    } // namespace Gait
} // namespace Move
