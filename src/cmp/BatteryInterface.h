#pragma once

#include "ComponentInterface.h"
#include "ComponentObservable.h"
#include "ComponentObserverInterface.h"

namespace Component {
enum BatteryState
{
	NOMINAL = 0x00U,
	WARNING,
	CRITICAL,
	UNKNOWN = 0xFFU
};

class BatteryInterface : public ComponentInterface, ComponentObservable <BatteryState> {
public:

	BatteryInterface( void ) = default;
	~BatteryInterface()      = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual BatteryState GetState() = 0;
	virtual uint16_t GetVoltage()   = 0;
};

class BatteryObserverInterface : public ComponentObserverInterface <BatteryState> {
public:
	BatteryObserverInterface()  = default;
	~BatteryObserverInterface() = default;

	virtual void UpdatedBatteryState( const BatteryState &batteryState ) = 0;

	inline virtual void UpdatedValue ( const BatteryState &batteryState ) final override
	{
		UpdatedBatteryState( batteryState );
	}
};
}
