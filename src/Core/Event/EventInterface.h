#pragma once

#include "EventId.h"

namespace Core {
class EventInterface
{
public:
	EventInterface()  = default;
	~EventInterface() = default;
	virtual void AddEvent( const EventId &event ) = 0;
};
}
