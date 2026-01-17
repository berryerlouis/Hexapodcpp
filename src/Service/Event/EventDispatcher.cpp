#include "EventDispatcher.h"
#include <algorithm>
namespace Service
{
    namespace Event
    {
        EventDispatcher::EventDispatcher()
            : mEventList() {
        }

        void EventDispatcher::AddListener(EventListenerInterface *listener) {
            if (listener) {
                this->mEventList.push_back(listener);
            }
        }

        void EventDispatcher::RemoveListener(EventListenerInterface *listener) {
            this->mEventList.erase(std::remove(this->mEventList.begin(),
                                               this->mEventList.end(),
                                               listener),
                                   this->mEventList.end());
        }

        void EventDispatcher::DispatchEvent(const Event &event) const {
            LOG_EVENT_DEBUG(
                    "%s(%d) Send Event %s.",
                    EServicesStruct::ServiceIdToString(event.serviceId).c_str(),
                    event.serviceId,
                    Event::Event::EventTypeToString(event.eventType).c_str());
            for (auto *listener: this->mEventList) {
                if (listener) {
                    listener->OnEvent(event);
                }
            }
        }
    } // namespace Event
} // namespace Service
