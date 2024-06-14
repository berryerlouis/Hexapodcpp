#pragma once

#include <stdint.h>
#include "../cmp/Communication.h"
#include "../clu/Frame.h"


class ServiceMediatorInterface {
public:
	virtual void SendFrame(Cluster::Frame &message) const = 0;
};

class BaseComponent {
protected:
	ServiceMediatorInterface *mServiceMediator;

public:
	BaseComponent(ServiceMediatorInterface *ServiceMediatorInterface = nullptr) : mServiceMediator(ServiceMediatorInterface)
	{
	}

	void SetComComponent (ServiceMediatorInterface *ServiceMediatorInterface)
	{
		this->mServiceMediator = ServiceMediatorInterface;
	}
};

class ServiceInterface {
public:
	ServiceInterface()  = default;
	~ServiceInterface() = default;

	virtual bool Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;
};
