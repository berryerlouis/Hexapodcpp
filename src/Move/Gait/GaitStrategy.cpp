#include "GaitStrategy.h"

#include <cmath>

namespace
{
    using LegPhaseTable = std::array<float, Bot::Legs::NB_LEGS>;

    // Swing start of each leg as a fraction of the full cycle, indexed by ELeg
    // (FRONT_LEFT, MIDDLE_LEFT, REAR_LEFT, FRONT_RIGHT, MIDDLE_RIGHT, REAR_RIGHT).

    // Two alternating tripods.
    constexpr LegPhaseTable TRIPOD_START = {0.0F, 0.5F, 0.0F, 0.5F, 0.0F, 0.5F};

    // One leg at a time, rear to front on the left side then on the right side.
    constexpr LegPhaseTable WAVE_START = {2.0F / 6.0F,
                                          1.0F / 6.0F,
                                          0.0F,
                                          5.0F / 6.0F,
                                          4.0F / 6.0F,
                                          3.0F / 6.0F};

    // Same-side legs 1/3 cycle apart, left and right sides half a cycle apart.
    constexpr LegPhaseTable RIPPLE_START = {2.0F / 3.0F,
                                            1.0F / 3.0F,
                                            0.0F,
                                            1.0F / 6.0F,
                                            5.0F / 6.0F,
                                            1.0F / 2.0F};

    float WrapUnit(const float value) {
        float wrapped = std::fmod(value, 1.0F);
        if (wrapped < 0.0F) {
            wrapped += 1.0F;
        }
        return wrapped;
    }

    void ApplyGait(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                   const Move::Gait::GaitParams              &gaitParams,
                   const Move::Gait::GaitTrajectory          &trajectory,
                   const float                                phase,
                   const float                                scale,
                   const LegPhaseTable                       &swingStart,
                   const float                                swingDuty) {
        const float cyclePhase = phase / Move::Gait::TRAJECTORY_SEGMENTS;

        for (auto &leg: legs) {
            const float legPhase =
                    WrapUnit(cyclePhase - swingStart[static_cast<size_t>(leg.second.GetId())]);

            size_t segment = 0U;
            float  time = legPhase / swingDuty;
            if (legPhase >= swingDuty) {
                segment = 1U;
                time = (legPhase - swingDuty) / (1.0F - swingDuty);
            }

            Misc::Maths::Position3d position = Misc::Utils::QuadraticLerp(
                    trajectory[segment][0U], trajectory[segment][1U], trajectory[segment][2U], time);
            leg.second.ComputeTarget(position,
                                     gaitParams.GetAmplitude() * scale,
                                     gaitParams.GetElevation() * scale,
                                     gaitParams.GetDirection(),
                                     gaitParams.IsRotated(),
                                     gaitParams.GetRotation(),
                                     gaitParams.GetRotationClockWize());
            leg.second.SetTarget(position);
        }
    }
} // namespace

namespace Move
{
    namespace Gait
    {
        void GaitTripod::doGaitStrategy(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                        const Move::Gait::GaitParams              &gaitParams,
                                        const GaitTrajectory                      &trajectory,
                                        const float                                phase,
                                        const float                                scale) {
            ApplyGait(legs, gaitParams, trajectory, phase, scale, TRIPOD_START, 0.5F);
        }

        void GaitWave::doGaitStrategy(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                      const Move::Gait::GaitParams              &gaitParams,
                                      const GaitTrajectory                      &trajectory,
                                      const float                                phase,
                                      const float                                scale) {
            ApplyGait(legs, gaitParams, trajectory, phase, scale, WAVE_START, 1.0F / 6.0F);
        }

        void GaitRipple::doGaitStrategy(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                        const Move::Gait::GaitParams              &gaitParams,
                                        const GaitTrajectory                      &trajectory,
                                        const float                                phase,
                                        const float                                scale) {
            ApplyGait(legs, gaitParams, trajectory, phase, scale, RIPPLE_START, 1.0F / 3.0F);
        }
    } // namespace Gait
} // namespace Move
