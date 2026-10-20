#pragma once

#include <array>
#include <map>
#include "../../Bot/Constants.h"
#include "../../Bot/Legs/LegsInterface.h"
#include "../../Misc/Maths/Utils.h"
#include "Constants.h"
#include "GaitParams.h"

namespace Move
{
    namespace Gait
    {
        // Segment 0 is the swing (foot in the air), segment 1 the stance (foot on the ground).
        // Each segment is a quadratic Bezier defined by 3 control points.
        using GaitTrajectory = std::array<std::array<Misc::Maths::Position3d, 3U>, 2U>;

        static constexpr float TRAJECTORY_SEGMENTS = 2.0F;

        class GaitStrategy {
        public:
            virtual ~GaitStrategy() = default;

            // phase is in [0, TRAJECTORY_SEGMENTS), scale in [0, 1] shrinks amplitude and elevation.
            virtual void doGaitStrategy(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                        const Move::Gait::GaitParams              &gaitParams,
                                        const GaitTrajectory                      &trajectory,
                                        const float                                phase,
                                        const float                                scale) = 0;
        };

        class GaitTripod : public GaitStrategy {
        public:
            void doGaitStrategy(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                const Move::Gait::GaitParams              &gaitParams,
                                const GaitTrajectory                      &trajectory,
                                const float                                phase,
                                const float                                scale) override;
        };

        class GaitWave : public GaitStrategy {
        public:
            void doGaitStrategy(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                const Move::Gait::GaitParams              &gaitParams,
                                const GaitTrajectory                      &trajectory,
                                const float                                phase,
                                const float                                scale) override;
        };

        class GaitRipple : public GaitStrategy {
        public:
            void doGaitStrategy(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                const Move::Gait::GaitParams              &gaitParams,
                                const GaitTrajectory                      &trajectory,
                                const float                                phase,
                                const float                                scale) override;
        };
    } // namespace Gait
} // namespace Move
