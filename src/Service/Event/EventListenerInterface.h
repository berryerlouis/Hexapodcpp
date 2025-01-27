#pragma once

#include "../../Cluster/Frame/Frame.h"

namespace Service
{
    namespace Event
    {
        class EventListenerInterface {
        public:
            EventListenerInterface() = default;

            ~EventListenerInterface() = default;

            virtual void SendMessage(const Cluster::Frame &message) const = 0;
        };
    }
}
