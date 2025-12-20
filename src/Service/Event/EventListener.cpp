#include "EventListener.h"

namespace Service
{
    namespace Event
    {
        EventListener::EventListener() : mEventList() {
        }

        EventType EventListener::GetEvent(void) {
            EventType event = EventType::EVENT_INVALID;
            if (!this->mEventList.empty()) {
                event = this->mEventList.front();
                this->mEventList.pop_front();
            }
            return event;
        }

        void EventListener::SetEvent(const EventType event) {
            this->mEventList.push_back(event);
        }

    } // namespace Event
} // namespace Service
