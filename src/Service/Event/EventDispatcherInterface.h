#pragma once

#include "SEvent.h"

namespace Service
{
    namespace Event
    {
#define MAX_EVENTS 10U

        class EventDispatcherInterface {
        public:
            EventDispatcherInterface() = default;

            ~EventDispatcherInterface() = default;

            virtual void DispatchEvent(const SEvent &event) = 0;
        };
    }
}
