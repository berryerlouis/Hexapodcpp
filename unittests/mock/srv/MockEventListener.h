#pragma once
#include <gmock/gmock.h>

#include "../../../src/Service/Event/EventListenerInterface.h"

namespace Service
{
	namespace Event
	{
		class MockEventListener : public EventListenerInterface {
		public:
			MOCK_METHOD1(AddEvent, void( const SEvent & ));
			MOCK_METHOD1(GetLastEvent, bool( SEvent & ));
		};
	}
}
