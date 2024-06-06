#pragma once

#include "DriverInterface.h"

class UartInterface : public DriverInterface
{
public:
	UartInterface(void) = default;
	~UartInterface()    = default;

	virtual void Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;

	virtual void Send(const char *data, const size_t len)    = 0;
	virtual void Send(const uint8_t *data, const size_t len) = 0;
	virtual void Send(const uint8_t data)     = 0;
	virtual uint8_t Read(void)                = 0;
	virtual uint8_t DataAvailable(void) const = 0;

private:
};
