#pragma once

#include "BufferEvent.h"

namespace Service
{
    namespace Event
    {
#define MAX_EVENTS 10U

        class EventListener {
        public:
            EventListener();

            ~EventListener() = default;

            void AddEvent(const SEvent &event);

            bool GetLastEvent(SEvent &event);

        private:
            BufferEvent mEventList;
        };
    }
}
