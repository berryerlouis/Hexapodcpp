#pragma once
#include <gmock/gmock.h>
#include "../../../src/Component/Adc/Ads1115Interface.h"

namespace Component
{
	namespace Adc
	{
		class MockAds1115 : public Ads1115Interface {
		public:
			MOCK_METHOD0(Initialize, Core::Status( void ));
			MOCK_METHOD1(Update, void( const uint64_t ));

			MOCK_METHOD0(GetVoltage, uint16_t( void ));
			MOCK_METHOD0(GetIntensity, uint16_t( void ));
		};
	}
}
