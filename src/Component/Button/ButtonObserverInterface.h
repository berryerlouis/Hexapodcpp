#pragma once

namespace Component
{
    namespace Proximity
    {
        class ButtonObserverInterface {
        public:
            ButtonObserverInterface() = default;

            ~ButtonObserverInterface() = default;

            virtual void Event(const uint16_t time) = 0;
        };
    }
}
