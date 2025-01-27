#pragma once
#include <gmock/gmock.h>

#include "../../../src/Service/Event/EventListenerInterface.h"

namespace Service
{
	namespace Event
	{
		class MockEventListener : public EventListenerInterface {
		public:
			MOCK_CONST_METHOD1(SendMessage, void( const Cluster::Frame & ));
		};
	}
}
