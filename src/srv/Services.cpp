#include "../srv/Services.h"
#include "../cor/Log.h"

Services::Services(
	CommunicationInterface &communication,
	ServiceGeneral &serviceGeneral,
	ServiceControl &serviceControl,
	ServiceProximity &serviceProximity,
	ServiceOrientation &serviceOrientation,
	ServiceBattery &serviceBattery,
	ServiceDisplay &serviceDisplay )
	: mCommunication( communication )
	, mServices{
					{ EServices::GENERAL, &serviceGeneral },
					{ EServices::CONTROL, &serviceControl },
					{ EServices::PROXIMITY, &serviceProximity },
					{ EServices::ORIUENTATION, &serviceOrientation },
					{ EServices::BATTERY, &serviceBattery },
					{ EServices::DISPLAY, &serviceDisplay }
					}
{
}

Core::CoreStatus Services::Initialize ( void )
{
	Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

	this->mCommunication.Initialize();
	for ( ServiceItem item : this->mServices )
	{
		item.service->SetComComponent( this );
		success = item.service->Initialize();
		if ( false == success )
		{
			LOG( "error" );
			LOG( item.serviceId );
		}
	}
	return ( success );
}

void Services::Update ( const uint64_t currentTime )
{
	this->mCommunication.Update( currentTime );
	for ( ServiceItem item : this->mServices )
	{
		if ( item.service->NeedUpdate( currentTime ) )
		{
			item.service->Update( currentTime );
			item.service->SetNewUpdateTime( currentTime );
		}
	}
}

void Services::SendFrame ( Frame &message ) const
{
	this->mCommunication.Send( message );
}

void Services::DisplayBatteryLevel ( Component::BatteryInterface::BatteryState state )
{
	ServiceDisplay *displayService = static_cast <ServiceDisplay *>( this->Get( EServices::DISPLAY ) );
	displayService->DisplayBatteryLevel( state );
}

void Services::DisplayProximitySensor ( Component::SensorProximityInterface::SensorsId sensor )
{
	ServiceDisplay *displayService = static_cast <ServiceDisplay *>( this->Get( EServices::DISPLAY ) );
	displayService->DisplayProximitySensor( sensor );
}

Service *Services::Get ( const EServices serviceId )
{
	for ( ServiceItem &item : this->mServices )
	{
		if ( item.serviceId == serviceId )
		{
			return ( item.service );
		}
	}
	return ( nullptr );
}
