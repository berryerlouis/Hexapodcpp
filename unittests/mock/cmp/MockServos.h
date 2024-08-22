#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Servos/ServosInterface.h"

namespace Component {
namespace Servos {
class MockServos : public ServosInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );
	MOCK_METHOD1( GetServo, Servo::Servo & ( const uint8_t ) );
	MOCK_METHOD1( GetServosController, ServosController::Pca9685Interface & ( const uint8_t ) );
};
}
}
