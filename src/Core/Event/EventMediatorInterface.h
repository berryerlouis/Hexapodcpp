#pragma once

#include "EventId.h"
namespace Core {
class EventMediatorInterface
{
public:
    EventMediatorInterface()  = default;
    ~EventMediatorInterface() = default;
    virtual void SendMessage( Event event ) const = 0;
};

class EventManager {
public:
    EventManager( EventMediatorInterface *mediator = nullptr )
        : mComMediator( mediator )
    {
    }

    void setMediator ( EventMediatorInterface *mediator )
    {
        this->mComMediator = mediator;
    }

protected:
    EventMediatorInterface *mComMediator;
};
}
