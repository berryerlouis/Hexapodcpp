#pragma once
#include <gmock/gmock.h>
#include "../../../src/Bot/Body/BodyInterface.h"


namespace Bot {
namespace Body {
class MockBody : public BodyInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD3( SetPositionRotation, void(const Leg::Position3d &, const Leg::Rotation3d &, const uint16_t ) );
};
}
}
