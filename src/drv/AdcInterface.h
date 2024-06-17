#pragma once

#include "DriverInterface.h"

namespace Driver {
class AdcInterface : public DriverInterface  {
public:
	AdcInterface()  = default;
	~AdcInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint32_t currentTime ) = 0;

	virtual void StartConversion( void ) = 0;
	virtual uint16_t Read( void )        = 0;
};
}
