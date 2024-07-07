#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/SoftwareInterface.h"

namespace Component {
class MockSoftware : public Component::SoftwareInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );
	MOCK_METHOD0( GetVersion, Version( void ) );
	MOCK_METHOD0( GetMinTime, uint64_t( void ) );
	MOCK_METHOD0( GetMaxTime, uint64_t( void ) );
	MOCK_METHOD0( ResetTiming, void(void) );
	MOCK_METHOD1( SetMinTime, void( const uint64_t ) );
	MOCK_METHOD1( SetMaxTime, void( const uint64_t ) );
};
}
