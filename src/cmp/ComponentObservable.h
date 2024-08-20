#pragma once
#include "../cor/CoreInterface.h"
#include "ComponentObservableInterface.h"

namespace Component {
template <typename ObjectType>
class ComponentObservable : public ComponentObservableInterface <ObjectType> {
public:
#define MAX_OBSERVERS    5U
	ComponentObservable() :
		mIndexList( 0U )
		, mListObserver{ nullptr }
	{
	}

	~ComponentObservable() = default;

	virtual Core::CoreStatus Attach ( ComponentObserverInterface <ObjectType> *observer ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
		if ( this->mIndexList != MAX_OBSERVERS )
		{
			this->mListObserver[this->mIndexList] = observer;
			this->mIndexList++;
			success = Core::CoreStatus::CORE_OK;
		}
		return ( success );
	}

	virtual void Notify ( const ObjectType &object ) final override
	{
		for ( size_t i = 0; i < this->mIndexList; i++ )
		{
			if ( this->mListObserver[i] != nullptr )
			{
				this->mListObserver[i]->UpdatedValue( object );
			}
		}
	}

private:
	uint8_t mIndexList;
	ComponentObserverInterface <ObjectType> *mListObserver[MAX_OBSERVERS];
};


template <typename ObjectType1, typename ObjectType2>
class ComponentObservable2 : public ComponentObservable2Interface <ObjectType1, ObjectType2> {
public:
#define MAX_OBSERVERS    5U
	ComponentObservable2() :
		mIndexList( 0U )
		, mListObserver{ nullptr }
	{
	}

	~ComponentObservable2() = default;

	virtual Core::CoreStatus Attach ( ComponentObserver2Interface <ObjectType1, ObjectType2> *observer ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;
		if ( this->mIndexList != MAX_OBSERVERS )
		{
			this->mListObserver[this->mIndexList] = observer;
			this->mIndexList++;
			success = Core::CoreStatus::CORE_OK;
		}
		return ( success );
	}

	virtual void Notify ( const ObjectType1 &object1, const ObjectType2 &object2 ) final override
	{
		for ( size_t i = 0; i < this->mIndexList; i++ )
		{
			if ( this->mListObserver[i] != nullptr )
			{
				this->mListObserver[i]->UpdatedValue( object1, object2 );
			}
		}
	}

private:
	uint8_t mIndexList;
	ComponentObserver2Interface <ObjectType1, ObjectType2> *mListObserver[MAX_OBSERVERS];
};
}
