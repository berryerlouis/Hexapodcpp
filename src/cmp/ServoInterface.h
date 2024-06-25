#pragma once

#include "ComponentInterface.h"

namespace Component {
class ServoInterface : public ComponentInterface {
public:
	ServoInterface()  = default;
	~ServoInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual Core::CoreStatus SetAngle( const uint8_t angle, const uint16_t travelTime = 0U ) = 0;
	virtual uint8_t GetAngle( void ) const        = 0;
	virtual bool SetMin( const uint8_t angle )    = 0;
	virtual uint8_t GetMin( void ) const          = 0;
	virtual bool SetMax( const uint8_t angle )    = 0;
	virtual uint8_t GetMax( void ) const          = 0;
	virtual bool SetOffset( const int8_t angle )  = 0;
	virtual int8_t GetOffset( void ) const        = 0;
	virtual void SetReverse( const bool reverse ) = 0;
	virtual bool GetReverse( void )             = 0;
	virtual void SetEnable( const bool enable ) = 0;
	virtual bool IsEnable( void ) = 0;
	virtual bool IsMoving( void ) = 0;
};
}
