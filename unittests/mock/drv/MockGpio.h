#pragma once
#include <gmock/gmock.h>
#include "../../../src/Driver/Gpio/GpioInterface.h"

namespace Driver {
namespace Gpio {
class MockGpio : public GpioInterface {
public:
	MOCK_METHOD0( Set, Core::CoreStatus( void ) );
	MOCK_METHOD0( Reset, Core::CoreStatus( void ) );
	MOCK_METHOD0( Get, bool( void ) );
	MOCK_METHOD0( GetPin, EPin & ( void ) );
	MOCK_METHOD0( SetInterruptPin, void( void ) );
	MOCK_METHOD0( ResetInterruptPin, void( void ) );
};
}
}
