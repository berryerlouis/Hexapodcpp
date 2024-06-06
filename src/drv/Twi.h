#pragma once

#include "DriverInterface.h"

class Twi : public DriverInterface {
public:
	enum EI2cFreq
	{
		FREQ_100_KHZ = 100000,
		FREQ_400_KHZ = 400000,
	};

	Twi(const EI2cFreq &freq);
	~Twi() = default;

	virtual void Initialize(void) final override;
	virtual void Update(const uint32_t currentTime) final override;

	bool ReadRegister(const uint8_t address, const uint8_t reg, uint8_t &data);
	bool ReadRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data);
	bool ReadRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data);
	bool ReadRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length);
	bool WriteRegister(const uint8_t address, const uint8_t reg, uint8_t data);
	bool WriteRegister16Bits(const uint8_t address, const uint8_t reg, uint16_t &data);
	bool WriteRegister32Bits(const uint8_t address, const uint8_t reg, uint32_t &data);
	bool WriteRegisters(const uint8_t address, const uint8_t reg, uint8_t *data, const uint8_t length);

private:
	enum EI2cStatus
	{
		ACK = 0,
		NACK,
		FAIL_ADDR,
		FAIL_START,
		FAIL_WRITE,
	};
	EI2cStatus Start(const uint8_t slave);
	EI2cStatus RepeatedStart(const uint8_t slave);
	EI2cStatus Write(const uint8_t data);
	uint8_t ReadAck(void);
	uint8_t ReadNack(void);
	EI2cStatus Stop(void);

	EI2cFreq mFreq;
	uint16_t mTimeout;
};
