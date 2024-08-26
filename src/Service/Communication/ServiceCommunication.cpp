#include "ServiceCommunication.h"
#include "../../Cluster/Battery/ClusterBattery.h"
#include "../../Cluster/General/ClusterGeneral.h"
#include "../../Cluster/Body/ClusterBody.h"
#include "../../Cluster/Imu/ClusterImu.h"
#include "../../Cluster/Proximity/ClusterProximity.h"
#include "../../Cluster/Servo/ClusterServo.h"

namespace Service {
namespace Communication {
ServiceCommunication::ServiceCommunication( CommunicationInterface &communication, Clusters::ClustersInterface &clusters )
	: Service( 1U )
	, mClusters( clusters )
	, mCommunication( communication )
{
}

Core::CoreStatus ServiceCommunication::Initialize ( void )
{
	return ( this->mCommunication.Initialize() );
}

void ServiceCommunication::Update ( const uint64_t currentTime )
{
	this->mCommunication.Update( currentTime );
}

void ServiceCommunication::Notify ( Core::Event event ) const
{
	Frame response;
	bool  success = false;
	switch ( event.id )
	{
	case Cluster::EClusters::GENERAL:
	{
		Cluster::General::ClusterGeneral *clusterGeneral = (Cluster::General::ClusterGeneral *) this->mClusters.GetCluster( Cluster::EClusters::GENERAL );

		if ( event.value == Cluster::EGeneralCommands::MIN_EXECUTION_TIME )
		{
			clusterGeneral->BuildFrameGetMinTime( response );
		}
		else if ( event.value == Cluster::EGeneralCommands::MAX_EXECUTION_TIME )
		{
			clusterGeneral->BuildFrameGetMaxTime( response );
		}
		success = true;
		break;
	} break;

	case Cluster::EClusters::IMU:
	{
		//success = true;
		break;
	} break;

	case Cluster::EClusters::PROXIMITY:
	{
		Cluster::Proximity::ClusterProximity *clusterProximity = (Cluster::Proximity::ClusterProximity *) this->mClusters.GetCluster( Cluster::EClusters::PROXIMITY );
		clusterProximity->BuildFrameDistance( (Cluster::EProximityCommands) event.value, response );
		success = true;
		break;
	}

	case Cluster::EClusters::SERVO:
	{
		//success = true;
		break;
	} break;

	case Cluster::EClusters::BATTERY:
	{
		Cluster::Battery::ClusterBattery *clusterBattery = (Cluster::Battery::ClusterBattery *) this->mClusters.GetCluster( Cluster::EClusters::BATTERY );
		clusterBattery->BuildFrameState( response );
		success = true;
		break;
	}
	break;

	case Cluster::EClusters::BODY:
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
