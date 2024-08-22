#pragma once

#include "../Cluster.h"
#include "../../Component/Proximity/SensorProximityInterface.h"

namespace Cluster {
namespace Proximity {
using namespace Component::Proximity;

class ClusterProximity : public Cluster {
public:
<<<<<<< HEAD
<<<<<<< HEAD
	ClusterProximity( SensorProximityMultipleInterface &proximity )
=======
	ClusterProximity( SensorProximityInterface &proximity )
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
=======
	ClusterProximity( SensorProximityMultipleInterface &proximity )
>>>>>>> 4ea6771 (create observers and update UT)
		: Cluster( PROXIMITY )
		, mProximity( proximity )
	{
	}

	~ClusterProximity() = default;

	inline virtual Core::CoreStatus Execute ( Frame &request, Frame &response ) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		if ( request.clusterId != this->GetId() )
		{
			return ( success );
		}

		switch ( (EProximityCommands) request.commandId )
		{
		case EProximityCommands::US_LEFT: {
			success = this->BuildFrameDistance( (EProximityCommands) request.commandId, response );
		}
		break;

		case EProximityCommands::US_RIGHT: {
			success = this->BuildFrameDistance( (EProximityCommands) request.commandId, response );
		}
		break;

		case EProximityCommands::LASER: {
			success = this->BuildFrameDistance( (EProximityCommands) request.commandId, response );
		}
		break;

		case EProximityCommands::SET_THRESHOLD: {
			uint8_t  sensorId  = request.Get1ByteParam( 0U );
			uint16_t threshold = request.Get2BytesParam( 1U );
<<<<<<< HEAD
<<<<<<< HEAD
			success = this->mProximity.SetThreshold( (SensorsId) sensorId, threshold );
=======
			success = this->mProximity.SetThreshold( (SensorProximityInterface::SensorsId) sensorId, threshold );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
=======
			success = this->mProximity.SetThreshold( (SensorsId) sensorId, threshold );
>>>>>>> 4ea6771 (create observers and update UT)
			if ( success == true )
			{
				success = this->BuildFrameThreshold( (EProximityCommands) request.commandId, response );
			}
		}
		break;

		default:
			break;
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameDistance ( EProximityCommands sensorId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::PROXIMITY,
			sensorId );
		if ( success )
		{
<<<<<<< HEAD
<<<<<<< HEAD
			response.Set2BytesParam( this->mProximity.GetDistance( (SensorsId) sensorId ) );
=======
			response.Set2BytesParam( this->mProximity.GetDistance( (SensorProximityInterface::SensorsId) sensorId ) );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
=======
			response.Set2BytesParam( this->mProximity.GetDistance( (SensorsId) sensorId ) );
>>>>>>> 4ea6771 (create observers and update UT)
		}
		return ( success );
	}

	inline Core::CoreStatus BuildFrameThreshold ( EProximityCommands sensorId, Frame &response )
	{
		Core::CoreStatus success = response.Build(
			EClusters::PROXIMITY,
			EProximityCommands::SET_THRESHOLD );
		if ( success )
		{
<<<<<<< HEAD
<<<<<<< HEAD
			response.Set2BytesParam( this->mProximity.GetThreshold( (SensorsId) sensorId ) );
=======
			response.Set2BytesParam( this->mProximity.GetThreshold( (SensorProximityInterface::SensorsId) sensorId ) );
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
=======
			response.Set2BytesParam( this->mProximity.GetThreshold( (SensorsId) sensorId ) );
>>>>>>> 4ea6771 (create observers and update UT)
		}
		return ( success );
	}

private:
<<<<<<< HEAD
<<<<<<< HEAD
	SensorProximityMultipleInterface &mProximity;
=======
	SensorProximityInterface &mProximity;
>>>>>>> f9be308 (create observer for Battery, used by service battery and service display, clusters are refactored to used the interface componenent)
=======
	SensorProximityMultipleInterface &mProximity;
>>>>>>> 4ea6771 (create observers and update UT)
};
}
}
