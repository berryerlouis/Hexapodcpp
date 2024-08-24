#include "Event.h"

namespace Core {
Event::Event()
	: mBufferEvent()
{
}

uint8_t Event::GetLength ( void ) const
{
	return ( this->mBufferEvent.GetLength( ) );
}

void Event::Push ( const EventId &event )
{
	this->mBufferEvent.Push( (const volatile uint8_t &) event );
}

EventId Event::Pop ( void )
{
	return ( (EventId) this->mBufferEvent.Pop( ) );
}
}
