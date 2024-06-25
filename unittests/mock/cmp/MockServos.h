#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/ServosInterface.h"

namespace Component {
class MockServos : public Component::ServosInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );
	MOCK_METHOD1( GetServo, Servo & ( const uint8_t ) );
	MOCK_METHOD1( GetPca9685, Pca9685Interface & ( const uint8_t ) );
};
}
