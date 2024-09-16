#pragma once

#include "EventListenerInterface.h"

namespace Service
{
    namespace Event
    {
#define MAX_EVENTS 10U

        class EventListener : public EventListenerInterface {
        public:
            EventListener();

            ~EventListener() = default;

            virtual void AddEvent(const SEvent &event) final override;

            virtual bool GetLastEvent(SEvent &event) final override;

        private:
            BufferEvent mEventList;
        };
    }
}
