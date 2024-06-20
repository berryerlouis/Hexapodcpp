#pragma once
#include <gmock/gmock.h>
#include "../../../src/cmp/ServoInterface.h"

namespace Component {
class MockServo : public Component::ServoInterface {
public:
	MOCK_METHOD0(Initialize, Core::CoreStatus(void) );
	MOCK_METHOD1(Update, void( const uint32_t ) );

	MOCK_METHOD2(SetAngle, Core::CoreStatus(const uint8_t, const uint16_t) );
	MOCK_CONST_METHOD0(GetAngle, uint8_t(void) );
	MOCK_METHOD1(SetMin, bool( const uint8_t) );
	MOCK_CONST_METHOD0(GetMin, uint8_t(void) );
	MOCK_METHOD1(SetMax, bool( const uint8_t) );
	MOCK_CONST_METHOD0(GetMax, uint8_t(void) );
	MOCK_METHOD1(SetOffset, bool( const int8_t) );
	MOCK_CONST_METHOD0(GetOffset, int8_t(void) );
	MOCK_METHOD1(SetReverse, void( const bool) );
	MOCK_METHOD0(GetReverse, bool( void) );
	MOCK_METHOD1(SetEnable, void( const bool) );
	MOCK_METHOD0(IsEnable, bool( void) );
	MOCK_METHOD0(IsMoving, bool( void) );
};
}
