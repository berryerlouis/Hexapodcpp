#pragma once

#include <stdint.h>
#include "GpioInterface.h"

namespace Driver {
class AdcInterface {
public:
	AdcInterface(void) = default;
	~AdcInterface()    = default;

	virtual bool Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;
	virtual void StartConversion(void) = 0;
	virtual uint16_t Read(void)        = 0;
};
}
