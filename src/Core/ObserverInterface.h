#pragma once

#include "stdint.h"

namespace Core
{
    template<class T>
    class ObserverInterface {
    public:
        ObserverInterface() = default;

        ~ObserverInterface() = default;

        virtual void Notified(const T &item) = 0;
    };
} // namespace Core
