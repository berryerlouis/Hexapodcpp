#pragma once

#include "ComponentInterface.h"

namespace Component {
class ProximityInterface : public ComponentInterface {
public:
	ProximityInterface(void) = default;
	~ProximityInterface()    = default;

	virtual Core::CoreStatus Initialize(void)       = 0;
	virtual void Update(const uint32_t currentTime) = 0;

	virtual uint16_t GetDistance(void)  = 0;
	virtual uint16_t GetThreshold(void) = 0;
	virtual Core::CoreStatus SetThreshold(const uint16_t threshold) = 0;
	virtual bool IsDetecting(void) = 0;

private:
};
}
