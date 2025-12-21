#include "EventListener.h"

namespace Service
{
    namespace Event
    {
        EventListener::EventListener() : mEventList() {
        }

        Event EventListener::GetEvent(void) {
            Event event = Event(EServices::NONE, Event::EventType::EVENT_NONE);
            if (!this->mEventList.empty()) {
                event = this->mEventList.front();
                this->mEventList.pop_front();
            }
            return event;
        }

        void EventListener::SetEvent(const EServices        serviceId,
                                     const Event::EventType eventType) {
            this->mEventList.push_back(Event(serviceId, eventType));
        }

    } // namespace Event
} // namespace Service
