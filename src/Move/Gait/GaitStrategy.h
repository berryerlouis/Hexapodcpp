#pragma once

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
        class GaitStrategy {
        public:
            virtual ~GaitStrategy() = default;

            virtual void
            doGaitStrategy(const bool                                isCycleComplete,
                           const uint64_t                            currentTime,
                           const uint64_t                            transitionStartTime,
                           std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                           const Move::Gait::GaitParams             &gaitParams,
                           const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                           const uint8_t stepPositionIndex,
                           const float   normalizedTime) = 0;
        };

        class GaitTripod : public GaitStrategy {
        public:
            void doGaitStrategy(const bool                                isCycleComplete,
                                const uint64_t                            currentTime,
                                const uint64_t                            transitionStartTime,
                                std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                const Move::Gait::GaitParams             &gaitParams,
                                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                                const uint8_t stepPositionIndex,
                                const float   normalizedTime) override;
        };

        class GaitWave : public GaitStrategy {
        public:
            void doGaitStrategy(const bool                                isCycleComplete,
                                const uint64_t                            currentTime,
                                const uint64_t                            transitionStartTime,
                                std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                const Move::Gait::GaitParams             &gaitParams,
                                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                                const uint8_t stepPositionIndex,
                                const float   normalizedTime) override;
        };

        class GaitRipple : public GaitStrategy {
        public:
            void doGaitStrategy(const bool                                isCycleComplete,
                                const uint64_t                            currentTime,
                                const uint64_t                            transitionStartTime,
                                std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &legs,
                                const Move::Gait::GaitParams             &gaitParams,
                                const std::vector<std::vector<Misc::Maths::Position3d>> &positions,
                                const uint8_t stepPositionIndex,
                                const float   normalizedTime) override;
        };
    } // namespace Gait
} // namespace Move
