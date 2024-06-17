#pragma once

#include "Service.h"

template <class T>
class ServiceCommon : public Service {
public:
	ServiceCommon( const uint32_t updateTime, T &cluster ) :
		Service( updateTime ),
		mCluster( cluster )
	{
	}

	~ServiceCommon() = default;

	inline virtual Core::CoreStatus Initialize ( void ) final override
	{
		return ( this->mCluster.Initialize() );
	}

	inline virtual void Update ( const uint32_t currentTime ) final override
	{
		this->mCluster.Update( currentTime );
	}

private:
	T &mCluster;
};
