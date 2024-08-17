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
<<<<<<< HEAD


template <typename ObjectType1, typename ObjectType2>
class ComponentObservable2Interface {
public:
	~ComponentObservable2Interface() = default;

	virtual Core::CoreStatus Attach( ComponentObserver2Interface <ObjectType1, ObjectType2> *observer ) = 0;
	virtual void Notify( const ObjectType1 &object1, const ObjectType2 &object2 ) = 0;
};
=======
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
}
