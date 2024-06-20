#pragma once

#include "ComponentInterface.h"

namespace Component {
class SoftwareInterface : public ComponentInterface {
public:
	struct Version
	{
		uint8_t major;
		uint8_t minor;
	};

	SoftwareInterface()  = default;
	~SoftwareInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint32_t currentTime ) = 0;

	virtual Version GetVersion( void )  = 0;
	virtual uint32_t GetMinTime( void ) = 0;
	virtual uint32_t GetMaxTime( void ) = 0;
	virtual void CheckTimingLoop( const uint32_t time ) = 0;
	virtual void ResetTiming( void ) = 0;

private:
};
}
