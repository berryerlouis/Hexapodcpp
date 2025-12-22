#pragma once

#include "Constants.h"

namespace Service
{
    namespace Event
    {
        class EventListenerInterface {
        public:
            EventListenerInterface() = default;

            ~EventListenerInterface() = default;

            virtual void OnEvent(const Event &event) = 0;
        };
    } // namespace Event
} // namespace Service
