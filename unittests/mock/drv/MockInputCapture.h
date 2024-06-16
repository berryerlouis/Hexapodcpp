#pragma once
#include <gmock/gmock.h>
#include "../../../src/drv/InputCaptureInterface.h"

namespace Driver {
class MockInputCapture : public Driver::InputCaptureInterface {
public:
	MOCK_METHOD0(Initialize, bool( void ) );
	MOCK_METHOD1(Update, void( const uint32_t ) );

	MOCK_METHOD0(EdgeChange, void( void ) );
	MOCK_METHOD0(GetInputCaptureTime, uint64_t(void) );
};
}
