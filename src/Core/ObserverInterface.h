#pragma once

namespace Core
{
    template<class T>
    class ObserverInterface {
    public:
        ObserverInterface() = default;

        virtual void Notified(const T &item) = 0;
    };
} // namespace Core
