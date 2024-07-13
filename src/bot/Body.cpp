#include "Body.h"

Body::Body( Legs &legs )
	: mBodyIk{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, { 0.0, 0.0, 0.0 } }
	, mLegs( legs )
{
}

Core::CoreStatus Body::Initialize ( void )
{
	Leg::Position3d position = { 0.0, 0.0, 0.0 };
	Leg::Rotation3d rotation = { 0.0, 0.0, 0.0 };

	this->SetPositionRotation( position, rotation, 1000 );
	return ( Core::CoreStatus::CORE_OK );
}

void Body::Update ( const uint64_t currentTime )
{
	(void) currentTime;
}

void Body::SetPositionRotation ( const Leg::Position3d &position, const Leg::Rotation3d &rotation, const uint16_t travelTime )
{
	this->SetBodyIk( position, rotation, travelTime );
}

void Body::SetBodyIk ( const Leg::Position3d &position, const Leg::Rotation3d &rotation, const uint16_t travelTime )
{
	for ( size_t legId = 0U; legId < NB_LEGS; legId++ )
	{
		Leg &leg = mLegs.GetLeg( legId );
		this->mBodyIk.totalX             = leg.mFootPosition.x + leg.mBodyCenterOffsetX + position.x;
		this->mBodyIk.totalY             = leg.mFootPosition.y + leg.mBodyCenterOffsetY + position.y;
		this->mBodyIk.distBodyCenterFeet = sqrt( this->mBodyIk.totalX * this->mBodyIk.totalX + this->mBodyIk.totalY * this->mBodyIk.totalY );
		this->mBodyIk.angleBodyCenterX   = atan2( this->mBodyIk.totalY, this->mBodyIk.totalX );
		this->mBodyIk.rollZ    = tan( rotation.angleZ * M_PI / 180.0 ) * this->mBodyIk.totalX;
		this->mBodyIk.pitchZ   = tan( rotation.angleX * M_PI / 180.0 ) * this->mBodyIk.totalY;
		this->mBodyIk.bodyIk.x = ( cos( this->mBodyIk.angleBodyCenterX + ( rotation.angleY * M_PI / 180.0 ) ) * this->mBodyIk.distBodyCenterFeet ) - this->mBodyIk.totalX;
		this->mBodyIk.bodyIk.y = ( sin( this->mBodyIk.angleBodyCenterX + ( rotation.angleY * M_PI / 180.0 ) ) * this->mBodyIk.distBodyCenterFeet ) - this->mBodyIk.totalY;
		this->mBodyIk.bodyIk.z = this->mBodyIk.rollZ + this->mBodyIk.pitchZ;
		leg.SetLegIk( position, this->mBodyIk.bodyIk, travelTime );
	}
}
