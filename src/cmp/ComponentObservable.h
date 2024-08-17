#pragma once
#include "../cor/CoreInterface.h"

namespace Component {
template <typename ObjectType>
class ComponentObservableInterface {
public:
	~ComponentObservableInterface() = default;

	virtual void Attach( ComponentObserver <ObjectType> *observer ) = 0;
	virtual void Notify( const ObjectType &object ) = 0;
};

/*template <typename ObjectType>
 * class ComponentObservable {
 * public:
 *      ComponentObservable() : mIndexList( 0U )
 *              , mListObserver{ nullptr }
 *      {
 *      }
 *
 *      ~ComponentObservable() = default;
 *
 *      void Attach ( ComponentObserver <ObjectType> *observer )
 *      {
 *              this->mListObserver[this->mIndexList] = observer;
 *      }
 *
 *      void Notify ( const ObjectType &object )
 *      {
 *              for ( size_t i = 0; i < this->mIndexList; i++ )
 *              {
 *                      this->mListObserver[i]->UpdatedValue( object );
 *              }
 *      }
 *
 * private:
 *      uint8_t mIndexList;
 *      ComponentObserver <ObjectType> *mListObserver[3U];
 * };*/
}
