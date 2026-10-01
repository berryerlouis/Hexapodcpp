#pragma once
#include <gmock/gmock.h>
#include "../../../src/Move/Gait/GaitStrategy.h"

namespace Move
{
    namespace Gait
    {
        class MockGaitStrategy : public GaitStrategy {
        public:
            MOCK_METHOD5(doGaitStrategy,
                         void(std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &,
                              const Move::Gait::GaitParams &,
                              const Move::Gait::GaitTrajectory &,
                              const float,
                              const float));
        };
    } // namespace Gait
} // namespace Move
