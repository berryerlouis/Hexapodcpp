#pragma once
#include "../cor/CoreInterface.h"
#include "ComponentObserverInterface.h"

namespace Component {
template <typename ObjectType>
class ComponentObservableInterface {
public:
	~ComponentObservableInterface() = default;

	virtual Core::CoreStatus Attach( ComponentObserverInterface <ObjectType> *observer ) = 0;
	virtual void Notify( const ObjectType &object ) = 0;
};
}
