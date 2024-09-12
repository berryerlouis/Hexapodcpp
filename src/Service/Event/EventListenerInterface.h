#pragma once

#include "EventId.h"

namespace Service
{
    namespace Event
    {
#define MAX_EVENTS 10U

        class EventListenerInterface {
        public:
            EventListenerInterface() = default;

            virtual ~EventListenerInterface() = default;

            virtual void DispatchEvent(SEvent &event) = 0;
        };
    }
}
