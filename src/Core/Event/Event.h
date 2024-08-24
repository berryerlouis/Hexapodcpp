#pragma once

#include "EventId.h"
#include "../../Misc/Buffer/Buffer.h"

namespace Core {
class Event
{
public:
	Event();
	~Event() = default;

	uint8_t GetLength( void ) const;
	void Push( const EventId &event );
	EventId Pop( void );

private:
	Misc::Buffer::Buffer mBufferEvent;
};
}
