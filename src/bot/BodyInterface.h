#pragma once

#include "Legs.h"

#include "../cmp/ComponentInterface.h"

class BodyInterface : public Component::ComponentInterface {
public:
	BodyInterface()  = default;
	~BodyInterface() = default;

	virtual Core::CoreStatus Initialize( void )       = 0;
	virtual void Update( const uint64_t currentTime ) = 0;

	virtual void SetPositionRotation( const Leg::Position3d &position, const Leg::Rotation3d &rotation, const uint16_t travelTime ) = 0;
};
