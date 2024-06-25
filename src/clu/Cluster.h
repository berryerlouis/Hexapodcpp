#pragma once

#include "ClusterInterface.h"
#include "../cmp/ComponentInterface.h"

namespace Clusters {
template <class T>
class Cluster : public ClusterInterface {
public:
	Cluster( EClusters clusterId, T &componentInterface )
		: mClusterId( clusterId )
		, mComponentInterface( componentInterface )
	{
	}

	inline virtual Core::CoreStatus Initialize ( void ) override
	{
		return ( mComponentInterface.Initialize() );
	}

	inline virtual void Update ( const uint64_t currentTime ) override
	{
		mComponentInterface.Update( currentTime );
	}

	virtual Core::CoreStatus Execute( Frame &request, Frame &response ) = 0;

	inline T & GetComponent ( void )
	{
		return ( mComponentInterface );
	}

	virtual EClusters GetId ( void )
	{
		return ( mClusterId );
	}

protected:
	const EClusters mClusterId;
	T &mComponentInterface;
};
}
