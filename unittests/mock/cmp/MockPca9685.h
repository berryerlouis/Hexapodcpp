#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/Pca9685Interface.h"

namespace Component {
class MockPca9685 : public Component::Pca9685Interface {
public:
	MOCK_METHOD0(Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1(Update, void( const uint32_t ) );

	MOCK_METHOD0(Reset, void( void ) );
	MOCK_METHOD0(Sleep, void( void ) );
	MOCK_METHOD0(WakeUp, void( void ) );
	MOCK_METHOD1(setOscillatorFrequency, void( const uint32_t  ) );
	MOCK_METHOD1(SetFrequency, void( const uint32_t  ) );
	MOCK_METHOD2(SetPwm, void(uint8_t, uint16_t) );
};
}
