#pragma once

#include "ComponentInterface.h"

namespace Component {
class LedInterface : public ComponentInterface {
public:
	enum LedState
	{
		ON = 0,
		OFF
	};

	LedInterface()  = default;
	~LedInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint32_t currentTime ) = 0;

	virtual Core::CoreStatus On()     = 0;
	virtual Core::CoreStatus Off()    = 0;
	virtual Core::CoreStatus Toggle() = 0;
	virtual LedState Get() const      = 0;
};
}
