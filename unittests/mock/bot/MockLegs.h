#pragma once
#include <gmock/gmock.h>
#include "../../../src/Bot/Legs/LegsInterface.h"

namespace Bot
{
    namespace Legs
    {
        class MockLegs : public LegsInterface {
        public:
            MOCK_CONST_METHOD1(GetLeg, Leg::LegInterface*( const ELeg ));
        };
    }
}
