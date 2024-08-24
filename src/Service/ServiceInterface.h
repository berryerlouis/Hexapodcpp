#pragma once

#include "../Component/Communication/Communication.h"
#include "../Cluster/Frame/Frame.h"

namespace Service {
using namespace Cluster;

class ServiceMediatorInterface
{
public:
	ServiceMediatorInterface()  = default;
	~ServiceMediatorInterface() = default;
	virtual void SendFrame( Frame &message ) const = 0;
};

class BaseComponent : public Core::CoreInterface {
protected:
	ServiceMediatorInterface *mServiceMediator;

public:
	BaseComponent( ServiceMediatorInterface *ServiceMediatorInterface = nullptr ) : mServiceMediator( ServiceMediatorInterface )
	{
	}

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	void SetComComponent ( ServiceMediatorInterface *ServiceMediatorInterface )
	{
		this->mServiceMediator = ServiceMediatorInterface;
	}
};

class ServiceInterface : public BaseComponent {
public:
	ServiceInterface()  = default;
	~ServiceInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;
};
}
