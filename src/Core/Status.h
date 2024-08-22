#pragma once

#include <stdint.h>

namespace Core {
struct CoreStatus
{
	static const int8_t CORE_ERROR = ( -1 );
	static const int8_t CORE_OK    = ( 0 );

	CoreStatus( int8_t state ) : status( state )
	{
	}

	operator bool() const
	{
		return ( status == CORE_OK );
	}
private:
	int8_t              status = CORE_ERROR;
};

class Utils
{
public:
};
}
