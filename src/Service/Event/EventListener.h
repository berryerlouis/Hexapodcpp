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

            virtual EventType GetEvent(void) final override;

            virtual void SetEvent(const EventType event) override;

        private:
            std::list<EventType> mEventList;
        };
    } // namespace Event
} // namespace Service
