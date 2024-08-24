#pragma once

#include "GpioInterface.h"

namespace Driver {
namespace Gpio {
class Gpio : public GpioInterface
{
public:
	Gpio( const SGpio &gpio, const EPortDirection &portDirection );
	~Gpio() = default;

	virtual Core::CoreStatus Set( void ) final override;
	virtual Core::CoreStatus Reset( void ) final override;
	virtual bool Get( void ) final override;
	virtual EPin &GetPin( void ) final override;
	virtual void SetInterruptPin( void ) final override;
	virtual void ResetInterruptPin( void ) final override;

private:
	struct GpioRegister
	{
		volatile uint8_t *pinPtr;
		volatile uint8_t *portPtr;
		volatile uint8_t *portDirPtr;
		volatile uint8_t *pcicr;
		volatile uint8_t *pcmsk;
		volatile uint8_t  pcie;
	};

	SGpio mGpio;
	EPortDirection mPortDirection;
	GpioRegister mGpioRegister;

	static GpioRegister sGpioRegisters[NB_PORT];
};
}
}