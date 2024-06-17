#pragma once
#include <stdint.h>
#include <stddef.h>

#include "Status.h"

namespace Core {
class CoreInterface {
public:
	CoreInterface(void) = default;
	~CoreInterface()    = default;

	virtual Core::CoreStatus Initialize(void)       = 0;
	virtual void Update(const uint32_t currentTime) = 0;
};
}
