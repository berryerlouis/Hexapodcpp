#pragma once
#include <stdint.h>
#include <stddef.h>

namespace Driver {
class DriverInterface {
public:
	DriverInterface(void) = default;
	~DriverInterface()    = default;

	virtual bool Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;
};
}
