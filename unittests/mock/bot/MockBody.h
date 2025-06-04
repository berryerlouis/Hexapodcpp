#pragma once
#include <gmock/gmock.h>
#include "../../../src/Bot/Body/BodyInterface.h"

namespace Bot
{
    namespace Body
    {
        class MockBody : public BodyInterface {
        public:
            MOCK_METHOD0(Initialize, Core::Status( void ));
            MOCK_METHOD1(Update, void( const uint64_t ));
            MOCK_METHOD1(UpdateWalkStatus, void( const EWalkStatus ));
            MOCK_METHOD3(SetBodyPositionRotation,
                         uint32_t (const Misc::Maths::Position3d &, const Misc::Maths::Rotation3d &, const uint16_t ));
            MOCK_METHOD3(SetLegPositionRotation,
                         uint32_t (const uint8_t &, const Misc::Maths::Position3d &, const uint16_t ));
            MOCK_METHOD1(SetDirection, bool( const uint16_t ));
            MOCK_METHOD1(SetAmplitude, bool( const uint8_t ));
            MOCK_METHOD1(SetElevation, bool( const uint8_t ));
            MOCK_CONST_METHOD0(GetDirection, uint16_t( void ));
            MOCK_CONST_METHOD0(GetAmplitude, uint8_t( void));
            MOCK_CONST_METHOD0(GetElevation, uint8_t( void));
        };
    }
}
