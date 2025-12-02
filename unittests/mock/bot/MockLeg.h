#pragma once
#include <gmock/gmock.h>
#include "../../../src/Bot/Leg/LegInterface.h"

namespace Bot
{
    namespace Leg
    {
        class MockLeg : public LegInterface {
        public:
            MOCK_METHOD1(SetTarget, void (const Position3d &));

            MOCK_METHOD0(ResetTarget, void (void));

            MOCK_METHOD0(Update, Core::Status (void));

            MOCK_CONST_METHOD0(GetId, Legs::ELeg (void));

            MOCK_METHOD2(SetLegIk, Core::Status (const Position3d &, const uint16_t));

            MOCK_METHOD3(SetLegBodyIk, Core::Status (const Position3d &, const Position3d &, const uint16_t));

            MOCK_CONST_METHOD2(ComputeDirection, void (Position3d &, float));

            MOCK_CONST_METHOD3(ComputeRotation, void (Position3d &, float, bool));

            MOCK_METHOD2(ComputeAmplitude, void (Position3d &, uint8_t));

            MOCK_METHOD2(ComputeElevation, void (Position3d &, uint8_t));

            MOCK_CONST_METHOD0(GetFootPosition, Position3d (void));

            MOCK_CONST_METHOD0(GetBodyCenterOffsetX, float (void));

            MOCK_CONST_METHOD0(GetBodyCenterOffsetY, float (void));
        };
    }
}