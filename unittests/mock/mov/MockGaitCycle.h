#pragma once
#include <gmock/gmock.h>
#include "../../../src/Move/Gait/GaitStrategy.h"

namespace Move
{
    namespace Gait
    {
        class MockGaitStrategy : public GaitStrategy {
        public:
            MOCK_CONST_METHOD7(doGaitStrategy,
                               void (const bool,
                                     const uint64_t,
                                     std::map<Bot::Legs::ELeg, Bot::Leg::Leg> &,
                                     const Move::Gait::GaitParams &,
                                     const std::vector<std::vector<Misc::Maths::Position3d> > &,
                                     const uint8_t,
                                     const float));
        };
    }
}