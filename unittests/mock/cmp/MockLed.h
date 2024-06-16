#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/LedInterface.h"

namespace Component {
class MockLed : public Component::LedInterface {
public:
	MOCK_METHOD0(Initialize, bool( void ) );
	MOCK_METHOD1(Update, void( const uint32_t ) );
	MOCK_METHOD0(On, bool( void ) );
	MOCK_METHOD0(Off, bool( void ) );
	MOCK_METHOD0(Toggle, bool( void ) );
	MOCK_CONST_METHOD0(Get, LedState(void) );
};
}
