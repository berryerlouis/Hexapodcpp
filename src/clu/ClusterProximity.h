#pragma once

#include "Cluster.h"
#include "../cmp/SensorProximityInterface.h"

namespace Clusters {
using namespace Component;

class ClusterProximity : public Cluster <SensorProximityInterface> {
public:
	ClusterProximity( SensorProximityInterface &proximity )
		: Cluster <SensorProximityInterface>( PROXIMITY, proximity )
	{
	}

	~ClusterProximity() = default;


	Core::CoreStatus Execute ( Frame &request, Frame &response ) final override
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
			SensorProximityInterface::SensorsId sensorId = (SensorProximityInterface::SensorsId) request.params[0U];
			uint16_t threshold = (uint16_t) ( request.params[2U] << 8U ) + request.params[1U];
			success = this->GetComponent().SetThreshold( sensorId, threshold );
			if ( success == Core::CoreStatus::CORE_OK )
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

	Core::CoreStatus BuildFrameDistance ( EProximityCommands side, Frame &response )
	{
		uint16_t distance = this->GetComponent().GetDistance( (SensorProximityInterface::SensorsId) side );
		uint8_t  params[] = { (uint8_t) ( distance >> 8U ), (uint8_t) ( distance & 0xFFU ) };
		return ( response.Build( EClusters::PROXIMITY, side, params, 2U ) );
	}

	Core::CoreStatus BuildFrameThreshold ( EProximityCommands side, Frame &response )
	{
		uint16_t threshold = this->GetComponent().GetThreshold( (SensorProximityInterface::SensorsId) side );
		uint8_t  params[]  = { (uint8_t) ( threshold >> 8U ), (uint8_t) threshold };
		return ( response.Build( EClusters::PROXIMITY, EProximityCommands::SET_THRESHOLD, params, 2U ) );
	}
};
}
