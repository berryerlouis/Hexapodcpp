#include "EventListener.h"

#include "../../Misc/Logger/Logger.h"

namespace Service
{
    namespace Event
    {
        EventListener::EventListener(): mEventList(MAX_EVENTS) {
        }

        void EventListener::AddEvent(const SEvent &event) {
            if (this->mEventList.IsFull()) {
                LOG("FULL LISTENER");
            }else
			{
				this->mEventList.Push(event);
			}
        }

        bool EventListener::GetLastEvent(SEvent &event) {
            return this->mEventList.Pop(event);
        }
    }
}
