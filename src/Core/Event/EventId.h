#pragma once

#include <stdint.h>
#include "../../Cluster/Constants.h"

namespace Core {
struct Event
{
	Cluster::EClusters id;
	uint8_t            value;
	bool operator== ( const Event & other ) const
	{
		return ( id == other.id && value == other.value );
	}
};
}
