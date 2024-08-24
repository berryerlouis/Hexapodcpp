#pragma once
#include "../Core/CoreInterface.h"
#include "ComponentObservableInterface.h"

namespace Component {

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
