#pragma once
#include <gmock/gmock.h>

#include "../../../src/Service/Event/EventDispatcherInterface.h"

namespace Service
{
	namespace Event
	{
		class MockEventDispatcher : public EventDispatcherInterface {
		public:
			MOCK_METHOD1(DispatchEvent, void( const SEvent & ));
		};
	}
}
