#pragma once

#include "BodyInterface.h"

class Body : public BodyInterface {
public:
	struct SBodyIk
	{
		float           totalY;
		float           totalX;
		float           distBodyCenterFeet;
		float           angleBodyCenterX;
		float           rollZ;
		float           pitchZ;
		Leg::Position3d bodyIk;
	};

	Body( Legs &legs );
	~Body() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint32_t currentTime ) final override;

	virtual void SetPositionRotation( const Leg::Position3d &position, const Leg::Rotation3d &rotation, const uint16_t travelTime ) final override;

private:
	SBodyIk mBodyIk;
	Legs &mLegs;

	void SetBodyIk( const Leg::Position3d &position, const Leg::Rotation3d &rotation, const uint16_t travelTime );
};
