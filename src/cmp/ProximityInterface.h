#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "../drv/DriverInterface.h"

class ProximityInterface : public DriverInterface {
public:
	ProximityInterface(void) = default;
	~ProximityInterface()    = default;

	virtual uint16_t GetDistance(void) = 0;
	virtual bool SetThreshold(const uint16_t threshold) = 0;
	virtual bool IsDetecting(void) = 0;

private:
};
