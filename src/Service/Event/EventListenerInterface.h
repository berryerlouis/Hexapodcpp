#pragma once

#include "../../Cluster/Frame/Frame.h"

namespace Service
{
    namespace Event
    {
        enum class EventType {
            EVENT_INIT,
            EVENT_INVALID

        };

        class EventListenerInterface {
        public:
            EventListenerInterface() = default;

            ~EventListenerInterface() = default;

            virtual EventType GetEvent(void) = 0;

            virtual void SetEvent(const EventType event) = 0;
        };
    } // namespace Event
} // namespace Service
