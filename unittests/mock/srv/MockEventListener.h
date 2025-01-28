#pragma once
#include <gmock/gmock.h>

#include "../../../src/Service/Event/MessageInterface.h"

namespace Service
{
	namespace Event
	{
		class MockEventListener : public MessageInterface {
		public:
			MOCK_CONST_METHOD1(SendMessage, void( const Cluster::Frame & ));
		};
	}
}
