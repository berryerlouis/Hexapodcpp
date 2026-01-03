#pragma once
#include <gmock/gmock.h>
#include "../../../src/Bot/Body/BodyInterface.h"

namespace Bot
{
    namespace Body
    {
        class MockBody : public BodyInterface {
        public:
            MOCK_METHOD0(Initialize, Core::Status(void));

            MOCK_METHOD1(Update, void(const uint64_t));

            MOCK_METHOD2(UpdateWalkStatus,
                         void(const Move::Walk::EWalkStatus, const uint16_t));

            MOCK_METHOD0(GetWalkStatus, Move::Walk::EWalkStatus(void));

            MOCK_METHOD3(SetBodyPositionRotation,
                         uint32_t(const Misc::Maths::Position3d &,
                                  const Misc::Maths::Rotation3d &,
                                  const uint16_t));

            MOCK_METHOD3(SetLegPositionRotation,
                         uint32_t(const uint8_t &,
                                  const Misc::Maths::Position3d &,
                                  const uint16_t));

            MOCK_METHOD1(SetDirection, bool(const float));

            MOCK_METHOD2(SetRotation, bool(const float, const bool));

            MOCK_METHOD1(SetAmplitude, bool(const float));

            MOCK_METHOD1(SetElevation, bool(const float));

            MOCK_METHOD1(SetDuration, bool(const uint16_t));

            MOCK_METHOD0(GetDirection, float(void));

            MOCK_METHOD0(GetRotation, float(void));

            MOCK_METHOD0(GetRotationClockWize, bool(void));

            MOCK_METHOD0(GetAmplitude, float(void));

            MOCK_METHOD0(GetElevation, float(void));

            MOCK_METHOD0(GetDuration, uint16_t(void));

            MOCK_METHOD1(SetGait, bool(const Move::Gait::GaitType));

            MOCK_METHOD0(GetGait, Move::Gait::GaitType(void));
        };
    } // namespace Body
} // namespace Bot
