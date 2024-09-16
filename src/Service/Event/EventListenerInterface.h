#pragma once

#include "BufferEvent.h"

namespace Service
{
    namespace Event
    {
#define MAX_EVENTS 10U

        class EventListenerInterface {
        public:
            EventListenerInterface() = default;

            ~EventListenerInterface() = default;

            virtual void AddEvent(const SEvent &event) = 0;

            virtual bool GetLastEvent(SEvent &event) = 0;
        };
    }
}
