#pragma once
#include <gmock/gmock.h>
#include "../../../src/Bot/Legs/LegsInterface.h"

namespace Bot
{
    namespace Legs
    {
        class MockLegs : public LegsInterface {
        public:
            MOCK_METHOD0(GetLegs, std::map<ELeg, Leg::Leg> &(void) );

            MOCK_METHOD1(GetLeg, Leg::LegInterface *(const ELeg));

            MOCK_METHOD0(Update, Core::Status(void));
        };
    } // namespace Legs
} // namespace Bot
