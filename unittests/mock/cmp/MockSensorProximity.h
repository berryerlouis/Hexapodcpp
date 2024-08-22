#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/SensorProximityInterface.h"

namespace Component {
class MockSensorProximity : public Component::SensorProximityMultipleInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD1( GetDistance, uint16_t( const SensorsId & ) );
	MOCK_METHOD1( GetThreshold, uint16_t( const SensorsId & ) );
	MOCK_METHOD2( SetThreshold, Core::CoreStatus( const SensorsId &, uint16_t ) );
};
}
