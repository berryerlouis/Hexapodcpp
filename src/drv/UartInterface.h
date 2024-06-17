#pragma once

#include "DriverInterface.h"

namespace Driver {
class UartInterface : public DriverInterface
{
public:
	UartInterface()  = default;
	~UartInterface() = default;

	virtual Core::CoreStatus Initialize(void) = 0;
	virtual void Update(const uint32_t currentTime) = 0;

	virtual void Send(const char *data, const size_t len)    = 0;
	virtual void Send(const uint8_t *data, const size_t len) = 0;
	virtual void Send(const uint8_t data)     = 0;
	virtual uint8_t Read(void)                = 0;
	virtual uint8_t DataAvailable(void) const = 0;

private:
};
}
