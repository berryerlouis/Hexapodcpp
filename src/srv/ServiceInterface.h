#pragma once

#include "../cmp/Communication.h"
#include "../clu/Frame.h"


class ServiceMediatorInterface {
public:
	ServiceMediatorInterface()  = default;
	~ServiceMediatorInterface() = default;
	virtual void SendFrame( Clusters::Frame &message ) const = 0;
};

class BaseComponent {
protected:
	ServiceMediatorInterface *mServiceMediator;

public:
	BaseComponent( ServiceMediatorInterface *ServiceMediatorInterface = nullptr ) : mServiceMediator( ServiceMediatorInterface )
	{
	}

	void SetComComponent ( ServiceMediatorInterface *ServiceMediatorInterface )
	{
		this->mServiceMediator = ServiceMediatorInterface;
	}
};

class ServiceInterface : public Core::CoreInterface {
public:
	ServiceInterface()  = default;
	~ServiceInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint32_t currentTime ) = 0;
};
