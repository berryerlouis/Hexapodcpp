#pragma once

#include "EventId.h"

namespace Core {
class EventMediatorInterface
{
	class EventManager;
public:
	EventMediatorInterface()  = default;
	~EventMediatorInterface() = default;
	virtual void Notify( Event event ) const = 0;
};

class EventManager {
public:
	EventManager( EventMediatorInterface *mediator = nullptr )
		: mMediator( mediator )
	{
	}

	void setMediator ( EventMediatorInterface *mediator )
	{
		this->mMediator = mediator;
	}

protected:
	EventMediatorInterface *mMediator;
};
}
