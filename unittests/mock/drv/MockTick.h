#pragma once
#include <gmock/gmock.h>
#include "../../../src/drv/TickInterface.h"

namespace Driver {
class MockTick : public Driver::TickInterface {
public:

	MOCK_METHOD0( GetUs, uint64_t( void ) );
	MOCK_METHOD0( GetMs, uint64_t( void ) );
	MOCK_METHOD1( DelayMs, void( uint64_t ) );
	MOCK_METHOD1( DelayUs, void( uint64_t ) );
};
}
