#pragma once
#include <gmock/gmock.h>

#include "../../../src/cmp/Mpu9150Interface.h"

namespace Component {
class MockMpu9150 : public Component::Mpu9150Interface {
public:
	MOCK_METHOD0( Initialize, Core::CoreStatus( void ) );
	MOCK_METHOD1( Update, void( const uint64_t ) );

	MOCK_METHOD0( ReadAcc, Vector3( void ) );
	MOCK_METHOD0( ReadGyr, Vector3( void ) );
	MOCK_METHOD0( ReadMag, Vector3( void ) );
	MOCK_METHOD0( ReadTemp, int16_t( void ) );
};
}
