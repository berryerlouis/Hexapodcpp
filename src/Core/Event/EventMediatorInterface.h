#pragma once

#include "EventId.h"

namespace Core
{
    class EventMediatorInterface {
    public:
        EventMediatorInterface() = default;

        ~EventMediatorInterface() = default;

        virtual void SendMessage(Event event) const = 0;
    };

    class EventManager {
    public:
        EventManager(const EventMediatorInterface *mediator = nullptr)
            : mComMediator(mediator) {
        }

        void setMediator(const EventMediatorInterface *mediator) {
            this->mComMediator = mediator;
        }

    protected:
        const EventMediatorInterface *mComMediator;
    };
}
