#pragma once
#include <gmock/gmock.h>

#include "../../../src/Service/Event/EventDispatcherInterface.h"

namespace Service
{
    namespace Event
    {
        class MockEventDispatcherInterface : public EventDispatcherInterface {
        public:
            MOCK_METHOD1(AddListener,
                         void(EventListenerInterface *));
            MOCK_METHOD1(RemoveListener,
                         void(EventListenerInterface *));
            MOCK_CONST_METHOD1(DispatchEvent,
                               void(const Event &));
        };
    } // namespace Event
} // namespace Service
