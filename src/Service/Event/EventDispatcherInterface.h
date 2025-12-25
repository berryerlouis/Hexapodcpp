#pragma once

#include "EventListenerInterface.h"

namespace Service
{
    namespace Event
    {
        class EventDispatcherInterface {
        public:
            EventDispatcherInterface() = default;

            ~EventDispatcherInterface() = default;

            virtual void AddListener(EventListenerInterface *listener) = 0;

            virtual void RemoveListener(EventListenerInterface *listener) = 0;

            virtual void DispatchEvent(const Event &event) const = 0;
        };
    } // namespace Event
} // namespace Service
