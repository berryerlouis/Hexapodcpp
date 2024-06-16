#pragma once

#include "../cmp/Mpu9150.h"
#include "Cluster.h"
#include "Constants.h"

namespace Cluster {
using namespace Component;
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
			success = this->BuildFrameAll(response);
			break;

		case EImuCommands::ACC:
			success = this->BuildFrameAcc(response);
			break;

		case EImuCommands::GYR:
			success = this->BuildFrameGyr(response);
			break;

		case EImuCommands::MAG:
			success = this->BuildFrameMag(response);
			break;

		case EImuCommands::TMP:
			success = this->BuildFrameTmp(response);
			break;

		default:
			break;
		}
		return (success);
	}

private:
	Mpu9150 &mMpu9150;

	bool BuildFrameAll (Frame &response)
	{
		uint8_t params[] = { 0, 0, 0 };

		response.Build(EClusters::IMU, EImuCommands::ALL, (uint8_t *) &params, 3U);
		return (true);
	}

	bool BuildFrameAcc (Frame &response)
	{
		Mpu9150::Vector3 params = this->mMpu9150.ReadAcc();

		response.Build(EClusters::IMU, EImuCommands::ACC, (uint8_t *) &params, 6U);
		return (true);
	}

	bool BuildFrameGyr (Frame &response)
	{
		Mpu9150::Vector3 params = this->mMpu9150.ReadGyr();

		response.Build(EClusters::IMU, EImuCommands::GYR, (uint8_t *) &params, 6U);
		return (true);
	}

	bool BuildFrameMag (Frame &response)
	{
		Mpu9150::Vector3 params = this->mMpu9150.ReadMag();

		response.Build(EClusters::IMU, EImuCommands::MAG, (uint8_t *) &params, 6U);
		return (true);
	}

	bool BuildFrameTmp (Frame &response)
	{
		int16_t params = this->mMpu9150.ReadTemp();

		response.Build(EClusters::IMU, EImuCommands::TMP, (uint8_t *) &params, 2U);
		return (true);
	}
};
}
