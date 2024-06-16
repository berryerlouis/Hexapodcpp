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

	LedInterface(void) = default;
	~LedInterface()    = default;

	virtual bool On()            = 0;
	virtual bool Off()           = 0;
	virtual bool Toggle()        = 0;
	virtual LedState Get() const = 0;
};
}
