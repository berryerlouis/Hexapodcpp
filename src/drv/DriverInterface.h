#pragma once
#include "../cor/CoreInterface.h"

namespace Driver {
class DriverInterface : public Core::CoreInterface {
public:
	DriverInterface()  = default;
	~DriverInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint32_t currentTime ) = 0;
};
}
