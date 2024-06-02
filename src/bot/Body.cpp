#include "Body.h"

Body::Body(Legs&legs)
	: mBodyIk{0U}
	, mLegs(legs) {
	Leg::Position3d position = { 0 };
	Leg::Rotation3d rotation = { 0 };

	this->SetPositionRotation(position, rotation, 1000);
}

void Body::SetPositionRotation (const Leg::Position3d&position, const Leg::Rotation3d&rotation, const uint16_t travelTime) {
	this->SetBodyIk(position, rotation, travelTime);
}

void Body::SetBodyIk (const Leg::Position3d&position, const Leg::Rotation3d&rotation, const uint16_t travelTime) {
	for (uint8_t legId = 0; legId < NB_LEGS; legId++) {
		this->mBodyIk.totalX             = mLegs.GetLeg(legId).mFootPosition.x + mLegs.GetLeg(legId).mBodyCenterOffsetX + position.x;
		this->mBodyIk.totalY             = mLegs.GetLeg(legId).mFootPosition.y + mLegs.GetLeg(legId).mBodyCenterOffsetY + position.y;
		this->mBodyIk.distBodyCenterFeet = sqrt(this->mBodyIk.totalX * this->mBodyIk.totalX + this->mBodyIk.totalY * this->mBodyIk.totalY);
		this->mBodyIk.angleBodyCenterX   = atan2(this->mBodyIk.totalY, this->mBodyIk.totalX);
		this->mBodyIk.rollZ    = tan(rotation.angleZ * M_PI / 180.0) * this->mBodyIk.totalX;
		this->mBodyIk.pitchZ   = tan(rotation.angleX * M_PI / 180.0) * this->mBodyIk.totalY;
		this->mBodyIk.bodyIk.x = (cos(this->mBodyIk.angleBodyCenterX + (rotation.angleY * M_PI / 180.0)) * this->mBodyIk.distBodyCenterFeet) - this->mBodyIk.totalX;
		this->mBodyIk.bodyIk.y = (sin(this->mBodyIk.angleBodyCenterX + (rotation.angleY * M_PI / 180.0)) * this->mBodyIk.distBodyCenterFeet) - this->mBodyIk.totalY;
		this->mBodyIk.bodyIk.z = this->mBodyIk.rollZ + this->mBodyIk.pitchZ;
		mLegs.GetLeg(legId).SetLegIk(position, this->mBodyIk.bodyIk, travelTime);
	}
}

bool Body::BuildFrameSetPosition (Frame&response) {
	uint8_t params[] = { true };

	response.Build(EClusters::BODY, EBodyCommands::SET_LEG_X_Y_Z, params, 1U);
	return(true);
}
