#pragma once

#include "UartInterface.h"
#include "Buffer.h"
namespace Driver {
class Uart : public UartInterface {
public:
	enum EBaudRate
	{
		BAUDRATE_115200 = 115200UL,
		BAUDRATE_500000 = 500000UL,
	};

	Uart(const EBaudRate & baud = BAUDRATE_115200);
	~Uart() = default;

	virtual bool Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	virtual void Send(const char *data, const size_t len) final override;
	virtual void Send(const uint8_t *data, const size_t len) final override;
	virtual void Send(const uint8_t data) final override;
	virtual uint8_t Read(void) final override;
	virtual uint8_t DataAvailable(void) const final override;

	static Buffer bufferRx;

private:
	EBaudRate mBaudRate;
};
}
