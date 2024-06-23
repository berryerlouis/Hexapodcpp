#include "../srv/Services.h"

Services::Services(
	CommunicationInterface &communication,
	ServiceGeneral &serviceGeneral,
	ServiceControl &serviceControl,
	ServiceProximity &serviceProximity,
	ServiceOrientation &serviceOrientation,
	ServiceBattery &serviceBattery )
	: mCommunication( communication )
	, mServices{
					&serviceGeneral,
					&serviceControl,
					&serviceProximity,
					&serviceOrientation,
					&serviceBattery
					}
{
}

Core::CoreStatus Services::Initialize ( void )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	mCommunication.Initialize();
	for ( Service *service : mServices )
	{
		service->SetComComponent( this );
		success = service->Initialize();
	}
	return ( success );
}

void Services::Update ( const uint32_t currentTime )
{
	mCommunication.Update( currentTime );
	for ( Service *service : mServices )
	{
		if ( service->NeedUpdate( currentTime ) )
		{
			service->Update( currentTime );
			service->SetNewUpdateTime( currentTime );
		}
	}
}

void Services::SendFrame ( Frame &message ) const
{
	this->mCommunication.Send( message );
}
