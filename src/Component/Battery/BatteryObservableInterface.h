#pragma once

#include "BatteryObserverInterface.h"
#include "../ComponentObservableInterface.h"

namespace Component {
namespace Battery {
class BatteryObservableInterface : public ComponentObservableInterface <BatteryObserverInterface, BatteryState> {
public:
	BatteryObservableInterface()  = default;
	~BatteryObservableInterface() = default;

	virtual Core::CoreStatus Attach( BatteryObserverInterface *observer ) = 0;
	virtual void Notify( const BatteryState &object ) = 0;
};
}
}
