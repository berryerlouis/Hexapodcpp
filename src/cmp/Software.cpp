#include "Software.h"

namespace Component {
Software::Software() : mMinTime( 0UL )
	, mMaxTime( 0UL )
{
}

Core::CoreStatus Software::Initialize ( void )
{
	return ( Core::CoreStatus::CORE_OK );
}

void Software::Update ( const uint32_t currentTime )
{
	(void) currentTime;
}

SoftwareInterface::Version Software::GetVersion ( void )
{
	SoftwareInterface::Version version;
	version.major = 1U;
	version.minor = 0U;
	return ( version );
}

uint32_t Software::GetMinTime ( void )
{
	return ( this->mMinTime );
}

uint32_t Software::GetMaxTime ( void )
{
	return ( this->mMaxTime );
}

void Software::ResetTiming ( void )
{
}

void Software::SetMinTime ( const uint32_t time )
{
	this->mMinTime = time;
}

void Software::SetMaxTime ( const uint32_t time )
{
	this->mMinTime = time;
}

void Software::CheckTimingLoop ( const uint32_t time )
{
	if ( time < this->mMinTime )
	{
		this->mMinTime = time;
	}
	else if ( time > this->mMaxTime )
	{
		this->mMaxTime = time;
	}
}
}
