#pragma once

#include "BodyInterface.h"

namespace Bot {
namespace Body {

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

	Body( Legs::Legs &legs );
	~Body() = default;

	virtual Core::CoreStatus Initialize( void ) final override;
	virtual void Update( const uint64_t currentTime ) final override;

	virtual void SetPositionRotation( const Position3d &position, const Rotation3d &rotation, const uint16_t travelTime ) final override;

private:
	SBodyIk mBodyIk;
	Legs::Legs &mLegs;

	void SetBodyIk( const Position3d &position, const Rotation3d &rotation, const uint16_t travelTime );
};
}
}
