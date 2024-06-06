#pragma once

#include "../clu/Constants.h"
#include "../clu/Frame.h"
#include "Legs.h"
#include <stddef.h>

#include <math.h>

class Body {
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

	Body(Legs &legs);
	~Body() = default;

	void SetPositionRotation(const Leg::Position3d &position, const Leg::Rotation3d &rotation, const uint16_t travelTime);

	bool BuildFrameSetPosition(Frame &response);

private:
	SBodyIk mBodyIk;
	Legs &mLegs;

	void SetBodyIk(const Leg::Position3d &position, const Leg::Rotation3d &rotation, const uint16_t travelTime);
};
