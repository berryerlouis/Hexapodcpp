#pragma once
#include <gmock/gmock.h>
#include "MockEventListener.h"
#include "MockEventDispatcher.h"
#include "../../../src/Service/ServiceInterface.h"


namespace Service
{
	class MockService : public ServiceInterface, public Event::EventDispatcherInterface {
	public:
		MockService(Event::EventListenerInterface &eventListener)
			: mEventListener(eventListener) {
		}

		MOCK_METHOD0(Initialize, Core::CoreStatus( void ));
		MOCK_METHOD1(Update, void( const uint64_t ));
		MOCK_METHOD1(DispatchEvent, void( SEvent & ));

	private:
		Event::EventListenerInterface &mEventListener;
	};
}
