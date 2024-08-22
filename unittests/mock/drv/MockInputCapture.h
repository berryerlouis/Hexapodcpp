#pragma once
#include <gmock/gmock.h>
#include "../../../src/Driver/InputCapture/InputCaptureInterface.h"

namespace Driver {
namespace InputCapture {
class MockInputCapture : public InputCaptureInterface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD0( EdgeChange, void( void ) );
	MOCK_METHOD0( GetInputCaptureTime, uint64_t( void ) );
};
}
}
