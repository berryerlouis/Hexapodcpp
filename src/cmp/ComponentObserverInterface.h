#pragma once
#include "../cor/CoreInterface.h"

namespace Component {
template <typename ObjectType>
class ComponentObserverInterface {
public:
	ComponentObserverInterface()  = default;
	~ComponentObserverInterface() = default;

	virtual void UpdatedValue( const ObjectType &object ) = 0;
};

template <typename ObjectType1, typename ObjectType2>
class ComponentObserver2Interface {
public:
	ComponentObserver2Interface()  = default;
	~ComponentObserver2Interface() = default;

	virtual void UpdatedValue( const ObjectType1 &object1, const ObjectType2 &object2 ) = 0;
};
}
