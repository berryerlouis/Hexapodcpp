#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/ProximityInterface.h"

namespace Component {
class MockProximity : public Component::ProximityInterface {
public:
	MOCK_METHOD0(Initialize, Core::CoreStatus(void) );
	MOCK_METHOD1(Update, void( const uint32_t ) );

	MOCK_METHOD0(GetDistance, uint16_t(void) );
	MOCK_METHOD0(GetThreshold, uint16_t(void) );
	MOCK_METHOD1(SetThreshold, Core::CoreStatus(uint16_t) );
	MOCK_METHOD0(IsDetecting, bool(void) );
};
}
