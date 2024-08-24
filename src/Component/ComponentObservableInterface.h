#pragma once
#include "../Core/CoreInterface.h"
#include "ComponentObserverInterface.h"

namespace Component {
template <typename ObserverType, typename ObjectNotifyType>
class ComponentObservableInterface {
public:
	~ComponentObservableInterface() = default;

	virtual Core::CoreStatus Attach( ObserverType *observer ) = 0;
	virtual void Notify( const ObjectNotifyType &object )     = 0;
};

template <typename ObjectType1, typename ObjectType2>
class ComponentObservable2Interface {
public:
	~ComponentObservable2Interface() = default;

	virtual Core::CoreStatus Attach( ComponentObserver2Interface <ObjectType1, ObjectType2> *observer ) = 0;
	virtual void Notify( const ObjectType1 &object1, const ObjectType2 &object2 ) = 0;
};
}
