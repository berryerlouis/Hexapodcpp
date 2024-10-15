#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Barometer/BarometerInterface.h"

namespace Component
{
	namespace Barometer
	{
		class MockBarometer : public BarometerInterface {
		public:
			MOCK_METHOD0(Initialize, Core::CoreStatus( void ));
			MOCK_METHOD1(Update, void( const uint64_t ));

			MOCK_METHOD0(GetTemp, int16_t( void ));
			MOCK_METHOD0(GetPressure, int32_t( void ));
			MOCK_METHOD0(GetAltitude, uint16_t( void ));
		};
	}
}
