#pragma once
#include <gmock/gmock.h>
#include "../../../src/drv/AdcInterface.h"

namespace Driver {
class MockAdc : public Driver::AdcInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD0( StartConversion, void( void ) );
	MOCK_METHOD0( Read, uint16_t( void ) );
};
}
