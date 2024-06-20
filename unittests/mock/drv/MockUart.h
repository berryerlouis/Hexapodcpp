#pragma once
#include <gmock/gmock.h>
#include "../../../src/drv/UartInterface.h"

namespace Driver {
class MockUart : public Driver::UartInterface {
public:

	MOCK_METHOD0(Initialize, Core::CoreStatus(void) );
	MOCK_METHOD1(Update, void( const uint32_t ) );

	MOCK_METHOD2(Send, void( const uint8_t *, const size_t ) );
	MOCK_METHOD2(Send, void( const char *, const size_t ) );
	MOCK_METHOD1(Send, void( const uint8_t ) );
	MOCK_METHOD0(Read, uint8_t(void) );
	MOCK_CONST_METHOD0(DataAvailable, uint8_t(void) );
};
}
