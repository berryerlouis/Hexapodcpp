#pragma once
#include "../Core/CoreInterface.h"

namespace Driver {
class DriverInterface : public Core::CoreInterface {
public:
	DriverInterface()  = default;
	~DriverInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;
};
}
