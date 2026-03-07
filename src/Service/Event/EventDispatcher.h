#pragma once

#include <vector>
#include "EventDispatcherInterface.h"

namespace Service
{
    namespace Event
    {
        class EventDispatcher : public EventDispatcherInterface {
        public:
            EventDispatcher();

            ~EventDispatcher() = default;

            virtual void AddListener(EventListenerInterface *listener) final override;

            virtual void RemoveListener(EventListenerInterface *listener) final override;

            virtual void DispatchEvent(const Event &event) const final override;

        private:
            std::vector<EventListenerInterface *> mEventList;
        };
    } // namespace Event
} // namespace Service
