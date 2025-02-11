#pragma once

#include "stdint.h"

namespace Component
{

    template<class T>
    class ObserverInterface {
    public:
        ObserverInterface() = default;

        ~ObserverInterface() = default;

        virtual void Notified(const T &item) = 0;
    };

}
