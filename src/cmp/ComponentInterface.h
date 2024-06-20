#pragma once
#include "../cor/CoreInterface.h"

namespace Component {
class ComponentInterface : public Core::CoreInterface {
public:
	ComponentInterface( void ) = default;
	~ComponentInterface()      = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint32_t currentTime ) = 0;
};
}
