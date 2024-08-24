#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#include "Status.h"

namespace Core {
class CoreInterface {
public:
	CoreInterface()  = default;
	~CoreInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;
};
}
