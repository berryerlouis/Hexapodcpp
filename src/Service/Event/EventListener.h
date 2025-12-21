#pragma once

#include <list>
#include "EventListenerInterface.h"

namespace Service
{
    namespace Event
    {
        class EventListener : public EventListenerInterface {
        public:
            EventListener();

            ~EventListener() = default;

            virtual Event GetEvent(void) final override;

            virtual void  SetEvent(const EServices        serviceId,
                                   const Event::EventType eventType) override;

        private:
            std::list<Event> mEventList;
        };
    } // namespace Event
} // namespace Service
