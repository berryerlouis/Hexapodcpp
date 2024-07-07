#pragma once

#include "stdint.h"
#include "stddef.h"
#include "ComponentInterface.h"

namespace Component {
class Pca9685Interface : public ComponentInterface {
public:
	Pca9685Interface( void ) = default;
	~Pca9685Interface()      = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual void Reset( void )  = 0;
	virtual void Sleep( void )  = 0;
	virtual void WakeUp( void ) = 0;
	virtual void setOscillatorFrequency( const uint32_t frequency ) = 0;
	virtual void SetFrequency( const uint32_t frequency )           = 0;
	virtual void SetPwm( uint8_t num, uint16_t off ) = 0;
};
}
