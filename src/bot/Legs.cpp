#include "Legs.h"

Legs::Legs(Servos &servos)
	: mLegFL(FRONT_LEFT,
				servos.GetServo(0),
				servos.GetServo(1),
				servos.GetServo(2) )
	, mLegML(MIDDLE_LEFT,
				servos.GetServo(3),
				servos.GetServo(4),
				servos.GetServo(5) )
	, mLegRL(REAR_LEFT,
				servos.GetServo(6),
				servos.GetServo(7),
				servos.GetServo(8) )
	, mLegFR(FRONT_RIGHT,
				servos.GetServo(15),
				servos.GetServo(16),
				servos.GetServo(17) )
	, mLegMR(MIDDLE_RIGHT,
				servos.GetServo(12),
				servos.GetServo(13),
				servos.GetServo(14) )
	, mLegRR(REAR_RIGHT,
				servos.GetServo(9),
				servos.GetServo(10),
				servos.GetServo(11) )
	, mLegs{&mLegFL, &mLegML, &mLegRL, &mLegFR, &mLegMR, &mLegRR}
{
}

Leg&Legs::GetLeg (const uint8_t legId)
{
	return (*this->mLegs[legId]);
}
