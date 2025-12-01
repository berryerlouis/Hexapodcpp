#pragma once
#include <gmock/gmock.h>
#include "../../../src/Bot/Legs/LegsInterface.h"

namespace Bot
{
    namespace Legs
    {
        class MockLegs : public LegsInterface {
        public:
            MOCK_METHOD1(GetLeg, Leg::LegInterface*( const ELeg ));
            MOCK_METHOD1(ResetLegs, void( const uint16_t ));
            MOCK_METHOD0(Update, Core::Status( void ));
            MOCK_METHOD4(SetTarget, void( const Move::Gait::GaitParams &,
                                          const std::vector<std::vector<Misc::Maths::Position3d>> &,
                                          const uint8_t, 
                                          const float ));
        };
    }
}
