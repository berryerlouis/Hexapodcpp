#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Proximity/SensorProximityInterface.h"

namespace Component {
namespace Proximity {
class MockSensorProximity : public SensorProximityMultipleInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD1( GetDistance, uint16_t( const SensorsId & ) );
	MOCK_METHOD1( GetThreshold, uint16_t( const SensorsId & ) );
	MOCK_METHOD2( SetThreshold, Core::CoreStatus( const SensorsId &, uint16_t ) );

	MOCK_METHOD1( Attach, Core::CoreStatus( SensorProximityObserverInterface *observer ) );
};
}
}
