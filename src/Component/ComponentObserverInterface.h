#pragma once
#include "../Core/CoreInterface.h"

namespace Component {
template <typename ObjectType>
class ComponentObserverInterface {
public:
	ComponentObserverInterface()  = default;
	~ComponentObserverInterface() = default;

	virtual void UpdatedValue( const ObjectType &object ) = 0;
};
}
