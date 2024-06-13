#pragma once

#include <stdint.h>
#include "../cmp/Communication.h"


class BaseComponent;
class ServiceMediator {
public:
	virtual void SendFrame(Frame &message) const = 0;
};

class BaseComponent {
protected:
	ServiceMediator *mServiceMediator;

public:
	BaseComponent(ServiceMediator *ServiceMediator = nullptr) : mServiceMediator(ServiceMediator)
	{
	}

	void SetComComponent (ServiceMediator *ServiceMediator)
	{
		this->mServiceMediator = ServiceMediator;
	}
};


class ServiceInterface {
public:
	ServiceInterface(void) = default;
	~ServiceInterface()    = default;

	virtual bool Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;
};
