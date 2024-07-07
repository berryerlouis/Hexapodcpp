#pragma once

#include "BatteryInterface.h"
#include "../drv/AdcInterface.h"

namespace Component {
using namespace Driver;

class Battery : public BatteryInterface {
public:
	Battery( AdcInterface &adc );
	~Battery() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual BatteryState GetState( void ) final override;
	virtual uint16_t     GetVoltage( void ) final override;

private:
	uint16_t mVoltage;
	BatteryState mState;
	AdcInterface &mAdc;
};
}
