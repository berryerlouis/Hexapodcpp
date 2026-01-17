#pragma once

#include "ObserverInterface.h"

namespace Core
{
    template<class T>
    class ObservableInterface {
    public:
        ObservableInterface() = default;

        virtual Core::Status Attach(ObserverInterface<T> *observer) = 0;

        virtual void         Notify(const T item) = 0;
    };
} // namespace Core
