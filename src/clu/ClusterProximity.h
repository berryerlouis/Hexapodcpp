#pragma once

#include "../snr/SensorProximity.h"
#include "Cluster.h"
#include "Constants.h"

namespace Cluster {
using namespace Component;
class ClusterProximity : public Cluster {
public:
	ClusterProximity(SensorProximity &sensorProximity)
		: Cluster(PROXIMITY)
		, mSensorProximity(sensorProximity)
	{
	}

	~ClusterProximity() = default;


	Core::CoreStatus Execute (Frame &request, Frame &response) final override
	{
		Core::CoreStatus success = Core::CoreStatus::CORE_ERROR;

		switch ( (EProximityCommands) request.commandId)
		{
		case EProximityCommands::US_LEFT: {
			success = this->BuildFrameDistance( (EProximityCommands) request.commandId, response);
		}
		break;

		case EProximityCommands::US_RIGHT: {
			success = this->BuildFrameDistance( (EProximityCommands) request.commandId, response);
		}
		break;

		case EProximityCommands::LASER: {
			success = this->BuildFrameDistance( (EProximityCommands) request.commandId, response);
		}
		break;

		case EProximityCommands::SET_THRESHOLD: {
			SensorProximity::SensorsId sensorId = (SensorProximity::SensorsId) request.params[0U];
			uint16_t threshold = (uint16_t) (request.params[2U] << 8U) + request.params[1U];
			success = this->mSensorProximity.SetThreshold(sensorId, threshold);
		}
		break;

		default:
			break;
		}
		return (success);
	}

private:
	SensorProximity &mSensorProximity;


	Core::CoreStatus BuildFrameDistance (EProximityCommands side, Frame &response)
	{
		uint16_t distance = this->mSensorProximity.GetDistance( (SensorProximity::SensorsId) side);
		uint8_t  params[] = { (uint8_t) (distance >> 8U), (uint8_t) (distance & 0xFFU) };

		response.Build(EClusters::PROXIMITY, side, params, 2U);
		return (Core::CoreStatus::CORE_OK);
	}
};
}
