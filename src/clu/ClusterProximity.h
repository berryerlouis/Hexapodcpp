#pragma once

#include "../snr/SensorProximity.h"
#include "Cluster.h"
#include "Constants.h"

class ClusterProximity : public Cluster {
public:
	ClusterProximity(SensorProximity&sensorProximity)
		: Cluster(PROXIMITY)
		, mSensorProximity(sensorProximity) {
	}

	~ClusterProximity() = default;


	bool Execute (Frame&request, Frame&response) final override {
		bool success = false;

		switch ((EProximityCommands)request.commandId) {
		case EProximityCommands::US_LEFT:
			success = this->mSensorProximity.BuildFrameDistance((EProximityCommands)request.commandId, response);
			break;

		case EProximityCommands::US_RIGHT:
			success = this->mSensorProximity.BuildFrameDistance((EProximityCommands)request.commandId, response);
			break;

		case EProximityCommands::LASER:
			success = this->mSensorProximity.BuildFrameDistance((EProximityCommands)request.commandId, response);
			break;

		case EProximityCommands::SET_THRESHOLD:
			SensorProximity::SensorsId sensor = (SensorProximity::SensorsId)request.params[0U];
			uint16_t threshold = (uint16_t)(request.params[2U] << 8U) + request.params[1U];
			success = this->mSensorProximity.SetThreshold(sensor, threshold);
			break;

		default:
			break;
		}
		return(success);
	}

private:
	SensorProximity&mSensorProximity;
};
