#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/BatteryInterface.h"

namespace Component {
class MockBattery : public Component::BatteryInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD0( GetState, BatteryState( void ) );
	MOCK_METHOD0( GetVoltage, uint16_t( void ) );
};
}
