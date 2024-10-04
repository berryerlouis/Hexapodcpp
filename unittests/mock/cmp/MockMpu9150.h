#pragma once
#include <gmock/gmock.h>

#include "../../../src/Component/Imu/Mpu9150Interface.h"

namespace Component
{
	namespace Imu
	{
		class MockMpu9150 : public Mpu9150Interface {
		public:
			MOCK_METHOD0(Initialize, Core::CoreStatus( void ));
			MOCK_METHOD1(Update, void( const uint64_t ));

			MOCK_METHOD0(ReadAcc, Vector3( void ));
			MOCK_METHOD0(ReadGyr, Vector3( void ));
			MOCK_METHOD0(ReadMag, Vector3( void ));
			MOCK_METHOD0(ReadTemp, int16_t( void ));
			MOCK_METHOD0(ReadYawPitchRoll, Position3D( void ));
		};
	}
}
