#pragma once
#include <gmock/gmock.h>
#include "../../../src/drv/GpioInterface.h"

namespace Driver {
class MockGpio : public Driver::GpioInterface {
public:
	MOCK_METHOD0(Set, bool( void ) );
	MOCK_METHOD0(Reset, bool( void ) );
	MOCK_METHOD0(Get, bool( void ) );
	MOCK_METHOD0(GetPin, EPin & (void) );
	MOCK_METHOD0(SetInterruptPin, void( void ) );
	MOCK_METHOD0(ResetInterruptPin, void( void ) );
};
}
