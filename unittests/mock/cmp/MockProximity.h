#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Proximity/SensorProximityInterface.h"

namespace Component {
namespace Proximity {
class MockProximity : public SensorProximityInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD0( GetDistance, uint16_t( void ) );
	MOCK_METHOD0( GetThreshold, uint16_t( void ) );
	MOCK_METHOD1( SetThreshold, Core::CoreStatus( uint16_t ) );
};
}
}
