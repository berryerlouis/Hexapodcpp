#pragma once

#include "../Constants.h"

namespace Service
{
    namespace Event
    {
        class Event {
        public:
            enum EventType {
                EVENT_INIT_SUCCESS,
                EVENT_INIT_FAILURE,
                EVENT_BUTTON_PRESSED,
                EVENT_BUTTON_RELEASED,
                EVENT_NONE,
                EVENT_INVALID
            };
            EventType eventType;
            EServices serviceId;
            Event(const EServices        serviceId,
                  const Event::EventType eventType) {
                this->serviceId = serviceId;
                this->eventType = eventType;
            }
        };

        class EventListenerInterface {
        public:
            EventListenerInterface() = default;

            ~EventListenerInterface() = default;

            virtual Event GetEvent(void) = 0;

            virtual void  SetEvent(const EServices        serviceId,
                                   const Event::EventType eventType) = 0;
        };
    } // namespace Event
} // namespace Service
