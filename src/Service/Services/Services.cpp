#include "Services.h"
#include "../../Cluster/Constants.h"
#include "../../Misc/Logger/Logger.h"

namespace Service {
namespace Services {
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
					{ EServices::PROXIMITY, &serviceProximity },
					{ EServices::CONTROL, &serviceControl },
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
		item.service->setMediator( this );
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

void Services::Notify ( Core::Event event ) const
{
	Frame response;
	bool  success = false;
	switch ( event.id )
	{
	case Cluster::GENERAL:
	{
		//this->mClusterGeneral.BuildFrameGetMinTime( response );
		//this->mClusterGeneral.BuildFrameGetMaxTime( response );
		//mCommunicationsuccess = true;
		break;
	} break;

	case Cluster::IMU:
	{
		//success = true;
		break;
	} break;

	case Cluster::PROXIMITY:
	{
		//this->mClusterProximity.BuildFrameDistance( (Cluster::EProximityCommands) sensorId, response );
		//success = true;
		break;
	}

	case Cluster::SERVO:
	{
		//success = true;
		break;
	} break;

	case Cluster::BATTERY:
	{
		//this->mClusterBattery.BuildFrameState( response );
		//success = true;
		break;
	}
	break;

	case Cluster::BODY:
	{
		//success = true;
		break;
	} break;

	default:
		break;
	}

	if ( true == success )
	{
		this->mCommunication.Send( response );
	}
}
}
}
