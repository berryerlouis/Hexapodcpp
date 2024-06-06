#pragma once

#include "../cmp/Mpu9150.h"
#include "Cluster.h"
#include "Constants.h"

class ClusterImu : public Cluster {
public:
	ClusterImu(Mpu9150 &mpu9150)
		: Cluster(IMU)
		, mMpu9150(mpu9150)
	{
	}

	~ClusterImu() = default;


	bool Execute (Frame &request, Frame &response) final override
	{
		bool success = false;

		switch ( (EImuCommands) request.commandId)
		{
		case EImuCommands::ALL:
			success = mMpu9150.BuildFrameAll(response);
			break;

		case EImuCommands::ACC:
			success = mMpu9150.BuildFrameAcc(response);
			break;

		case EImuCommands::GYR:
			success = mMpu9150.BuildFrameGyr(response);
			break;

		case EImuCommands::MAG:
			success = mMpu9150.BuildFrameMag(response);
			break;

		case EImuCommands::TMP:
			success = mMpu9150.BuildFrameTmp(response);
			break;

		default:
			break;
		}
		return (success);
	}

private:
	Mpu9150 &mMpu9150;
};
